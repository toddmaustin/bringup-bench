#include "libmin.h"
#include "libtarg.h"

/*
 * Copyright Patrick Powell 1995
 * This code is based on code written by Patrick Powell (papowell@astart.com)
 * It may be used for any purpose as long as this notice remains intact
 * on all source code distributions
 */

/**************************************************************
 * Original:
 * Patrick Powell Tue Apr 11 09:48:21 PDT 1995
 * A bombproof version of doprnt (dopr) included.
 * Sigh.  This sort of thing is always nasty do deal with.  Note that
 * the version here does not include floating point...
 *
 * snprintf() is used instead of sprintf() as it does limit checks
 * for string length.  This covers a nasty loophole.
 *
 * The other functions are there to prevent NULL pointers from
 * causing nast effects.
 *
 * More Recently:
 *  Brandon Long <blong@fiction.net> 9/15/96 for mutt 0.43
 *  This was ugly.  It is still ugly.  I opted out of floating point
 *  numbers, but the formatter understands just about everything
 *  from the normal C string format, at least as far as I can tell from
 *  the Solaris 2.5 printf(3S) man page.
 *
 *  Brandon Long <blong@fiction.net> 10/22/97 for mutt 0.87.1
 *    Ok, added some minimal floating point support, which means this
 *    probably requires libm on most operating systems.  Don't yet
 *    support the exponent (e,E) and sigfig (g,G).  Also, fmtint()
 *    was pretty badly broken, it just wasn't being exercised in ways
 *    which showed it, so that's been fixed.  Also, formated the code
 *    to mutt conventions, and removed dead code left over from the
 *    original.  Also, there is now a builtin-test, just compile with:
 *           gcc -DTEST_SNPRINTF -o snprintf snprintf.c -lm
 *    and run snprintf for results.
 * 
 *  Thomas Roessler <roessler@guug.de> 01/27/98 for mutt 0.89i
 *    The PGP code was using unsigned hexadecimal formats. 
 *    Unfortunately, unsigned formats simply didn't work.
 *
 *  Michael Elkins <me@cs.hmc.edu> 03/05/98 for mutt 0.90.8
 *    The original code assumed that both snprintf() and vsnprintf() were
 *    missing.  Some systems only have snprintf() but not vsnprintf(), so
 *    the code is now broken down under HAVE_SNPRINTF and HAVE_VSNPRINTF.
 *
 *  Andrew Tridgell (tridge@samba.org) Oct 1998
 *    fixed handling of %.0f
 *    added test for HAVE_LONG_DOUBLE
 *
 * tridge@samba.org, idra@samba.org, April 2001
 *    got rid of fcvt code (twas buggy and made testing harder)
 *    added C99 semantics
 *
 **************************************************************/

#define HAVE_LONG_DOUBLE
#define HAVE_LONG_LONG

#ifdef HAVE_LONG_DOUBLE
#define LDOUBLE long double
#else
#define LDOUBLE double
#endif

#ifdef HAVE_LONG_LONG
#define LLONG long long
#else
#define LLONG long
#endif

static size_t
dopr(char *buffer, size_t maxlen, const char *format, 
     va_list args);

static void
fmtstr(char *buffer, size_t *currlen, size_t maxlen,
       char *value, int flags, int min, int max);

static void
fmtint(char *buffer, size_t *currlen, size_t maxlen,
       long value, int base, int min, int max, int flags);

static void
fmtfp(char *buffer, size_t *currlen, size_t maxlen,
      LDOUBLE fvalue, int min, int max, int flags);

static void
dopr_outch(char *buffer, size_t *currlen, size_t maxlen, char c);

/*
 * dopr(): poor man's version of doprintf
 */

/* format read states */
#define DP_S_DEFAULT 0
#define DP_S_FLAGS   1
#define DP_S_MIN     2
#define DP_S_DOT     3
#define DP_S_MAX     4
#define DP_S_MOD     5
#define DP_S_CONV    6
#define DP_S_DONE    7

/* format flags - Bits */
#define DP_F_MINUS 	(1 << 0)
#define DP_F_PLUS  	(1 << 1)
#define DP_F_SPACE 	(1 << 2)
#define DP_F_NUM   	(1 << 3)
#define DP_F_ZERO  	(1 << 4)
#define DP_F_UP    	(1 << 5)
#define DP_F_UNSIGNED 	(1 << 6)

/* Conversion Flags */
#define DP_C_SHORT   1
#define DP_C_LONG    2
#define DP_C_LDOUBLE 3
#define DP_C_LLONG   4

#define char_to_int(p) ((p)- '0')
#ifndef MAX
#define MAX(p,q) (((p) >= (q)) ? (p) : (q))
#endif

static size_t
dopr(char *buffer, size_t maxlen, const char *format, va_list args)
{
  char ch;
  LLONG value;
  LDOUBLE fvalue;
  char *strvalue;
  int min;
  int max;
  int state;
  int flags;
  int cflags;
  size_t currlen;
	
  state = DP_S_DEFAULT;
  currlen = flags = cflags = min = 0;
  max = -1;
  ch = *format++;

  while (state != DP_S_DONE) {
    if (ch == '\0') 
      state = DP_S_DONE;

    switch(state) {
    case DP_S_DEFAULT:
      if (ch == '%') 
	state = DP_S_FLAGS;
      else 
	dopr_outch (buffer, &currlen, maxlen, ch);
      ch = *format++;
      break;
    case DP_S_FLAGS:
      switch (ch) {
      case '-':
	flags |= DP_F_MINUS;
	ch = *format++;
	break;
      case '+':
	flags |= DP_F_PLUS;
	ch = *format++;
	break;
      case ' ':
	flags |= DP_F_SPACE;
	ch = *format++;
	break;
      case '#':
	flags |= DP_F_NUM;
	ch = *format++;
	break;
      case '0':
	flags |= DP_F_ZERO;
	ch = *format++;
	break;
      default:
	state = DP_S_MIN;
	break;
      }
      break;
    case DP_S_MIN:
      if (isdigit((unsigned char)ch)) {
	min = 10*min + char_to_int (ch);
	ch = *format++;
      } else if (ch == '*') {
	min = va_arg (args, int);
	ch = *format++;
	state = DP_S_DOT;
      } else {
	state = DP_S_DOT;
      }
      break;
    case DP_S_DOT:
      if (ch == '.') {
	state = DP_S_MAX;
	ch = *format++;
      } else { 
	state = DP_S_MOD;
      }
      break;
    case DP_S_MAX:
      if (isdigit((unsigned char)ch)) {
	if (max < 0)
	  max = 0;
	max = 10*max + char_to_int (ch);
	ch = *format++;
      } else if (ch == '*') {
	max = va_arg (args, int);
	ch = *format++;
	state = DP_S_MOD;
      } else {
	state = DP_S_MOD;
      }
      break;
    case DP_S_MOD:
      switch (ch) {
      case 'h':
	cflags = DP_C_SHORT;
	ch = *format++;
	break;
      case 'l':
	cflags = DP_C_LONG;
	ch = *format++;
	if (ch == 'l') {	/* It's a long long */
	  cflags = DP_C_LLONG;
	  ch = *format++;
	}
	break;
      case 'L':
	cflags = DP_C_LDOUBLE;
	ch = *format++;
	break;
      default:
	break;
      }
      state = DP_S_CONV;
      break;
    case DP_S_CONV:
      switch (ch) {
      case 'd':
      case 'i':
	if (cflags == DP_C_SHORT) 
	  value = va_arg (args, int);
	else if (cflags == DP_C_LONG)
	  value = va_arg (args, long int);
	else if (cflags == DP_C_LLONG)
	  value = va_arg (args, LLONG);
	else
	  value = va_arg (args, int);
	fmtint (buffer, &currlen, maxlen, value, 10, min, max, flags);
	break;
      case 'o':
	flags |= DP_F_UNSIGNED;
	if (cflags == DP_C_SHORT)
	  value = va_arg (args, unsigned int);
	else if (cflags == DP_C_LONG)
	  value = (long)va_arg (args, unsigned long int);
	else if (cflags == DP_C_LLONG)
	  value = (long)va_arg (args, unsigned LLONG);
	else
	  value = (long)va_arg (args, unsigned int);
	fmtint (buffer, &currlen, maxlen, value, 8, min, max, flags);
	break;
      case 'u':
	flags |= DP_F_UNSIGNED;
	if (cflags == DP_C_SHORT)
	  value = va_arg (args, unsigned int);
	else if (cflags == DP_C_LONG)
	  value = (long)va_arg (args, unsigned long int);
	else if (cflags == DP_C_LLONG)
	  value = (LLONG)va_arg (args, unsigned LLONG);
	else
	  value = (long)va_arg (args, unsigned int);
	fmtint (buffer, &currlen, maxlen, value, 10, min, max, flags);
	break;
      case 'X':
	flags |= DP_F_UP;
      case 'x':
	flags |= DP_F_UNSIGNED;
	if (cflags == DP_C_SHORT)
	  value = va_arg (args, unsigned int);
	else if (cflags == DP_C_LONG)
	  value = (long)va_arg (args, unsigned long int);
	else if (cflags == DP_C_LLONG)
	  value = (LLONG)va_arg (args, unsigned LLONG);
	else
	  value = (long)va_arg (args, unsigned int);
	fmtint (buffer, &currlen, maxlen, value, 16, min, max, flags);
	break;
      case 'f':
	if (cflags == DP_C_LDOUBLE)
	  fvalue = va_arg (args, LDOUBLE);
	else
	  fvalue = va_arg (args, double);
	/* um, floating point? */
	fmtfp (buffer, &currlen, maxlen, fvalue, min, max, flags);
	break;
      case 'E':
	flags |= DP_F_UP;
      case 'e':
	if (cflags == DP_C_LDOUBLE)
	  fvalue = va_arg (args, LDOUBLE);
	else
	  fvalue = va_arg (args, double);
	break;
      case 'G':
	flags |= DP_F_UP;
      case 'g':
	if (cflags == DP_C_LDOUBLE)
	  fvalue = va_arg (args, LDOUBLE);
	else
	  fvalue = va_arg (args, double);
	break;
      case 'c':
	dopr_outch (buffer, &currlen, maxlen, va_arg (args, int));
	break;
      case 's':
	strvalue = va_arg (args, char *);
	if (max == -1) {
	  max = libmin_strlen(strvalue);
	}
	if (min > 0 && max >= 0 && min > max) max = min;
	fmtstr (buffer, &currlen, maxlen, strvalue, flags, min, max);
	break;
      case 'p':
	strvalue = va_arg (args, void *);
	fmtint (buffer, &currlen, maxlen, (long)strvalue, 16, min, max, flags);
	break;
      case 'n':
	if (cflags == DP_C_SHORT) {
	  short int *num;
	  num = va_arg (args, short int *);
	  *num = currlen;
	} else if (cflags == DP_C_LONG) {
	  long int *num;
	  num = va_arg (args, long int *);
	  *num = (long int)currlen;
	} else if (cflags == DP_C_LLONG) {
	  LLONG *num;
	  num = va_arg (args, LLONG *);
	  *num = (LLONG)currlen;
	} else {
	  int *num;
	  num = va_arg (args, int *);
	  *num = currlen;
	}
	break;
      case '%':
	dopr_outch (buffer, &currlen, maxlen, ch);
	break;
      case 'w':
	/* not supported yet, treat as next char */
	ch = *format++;
	break;
      default:
	/* Unknown, skip */
	break;
      }
      ch = *format++;
      state = DP_S_DEFAULT;
      flags = cflags = min = 0;
      max = -1;
      break;
    case DP_S_DONE:
      break;
    default:
      /* hmm? */
      break; /* some picky compilers need this */
    }
  }
  if (maxlen != 0) {
    if (currlen < maxlen - 1) 
      buffer[currlen] = '\0';
    else if (maxlen > 0) 
      buffer[maxlen - 1] = '\0';
  }
  return currlen;
}

static void
fmtstr(char *buffer, size_t *currlen, size_t maxlen,
       char *value, int flags, int min, int max)
{
	int padlen, strln;     /* amount to pad */
	int cnt = 0;

#ifdef DEBUG_SNPRINTF
	printf("fmtstr min=%d max=%d s=[%s]\n", min, max, value);
#endif
	if (value == 0) {
	  value = "<NULL>";
	}

	for (strln = 0; value[strln]; ++strln); /* strlen */
	padlen = min - strln;
	if (padlen < 0) 
	  padlen = 0;
	if (flags & DP_F_MINUS) 
	  padlen = -padlen; /* Left Justify */
	
	while ((padlen > 0) && (cnt < max)) {
	  dopr_outch (buffer, currlen, maxlen, ' ');
	  --padlen;
	  ++cnt;
	}
	while (*value && (cnt < max)) {
	  dopr_outch (buffer, currlen, maxlen, *value++);
	  ++cnt;
	}
	while ((padlen < 0) && (cnt < max)) {
	  dopr_outch (buffer, currlen, maxlen, ' ');
	  ++padlen;
	  ++cnt;
	}
}

/* Have to handle DP_F_NUM (ie 0x and 0 alternates) */

static void
fmtint(char *buffer, size_t *currlen, size_t maxlen,
       long value, int base, int min, int max, int flags)
{
  int signvalue = 0;
  unsigned long uvalue;
  char convert[20];
  int place = 0;
  int spadlen = 0; /* amount to space pad */
  int zpadlen = 0; /* amount to zero pad */
  int caps = 0;
	
  if (max < 0)
    max = 0;
	
  uvalue = value;

  if(!(flags & DP_F_UNSIGNED)) {
    if( value < 0 ) {
      signvalue = '-';
      uvalue = -value;
    } else {
      if (flags & DP_F_PLUS)  /* Do a sign (+/i) */
	signvalue = '+';
      else if (flags & DP_F_SPACE)
	signvalue = ' ';
    }
  }
  
  if (flags & DP_F_UP)
    caps = 1; /* Should characters be upper case? */
  
  do {
    convert[place++] =
      (caps? "0123456789ABCDEF":"0123456789abcdef")
      [uvalue % (unsigned)base  ];
    uvalue = (uvalue / (unsigned)base );
  } while(uvalue && (place < 20));
  if (place == 20) place--;
  convert[place] = 0;

  zpadlen = max - place;
  spadlen = min - MAX (max, place) - (signvalue ? 1 : 0);
  if (zpadlen < 0) zpadlen = 0;
  if (spadlen < 0) spadlen = 0;
  if (flags & DP_F_ZERO) {
    zpadlen = MAX(zpadlen, spadlen);
    spadlen = 0;
  }
  if (flags & DP_F_MINUS) 
    spadlen = -spadlen; /* Left Justifty */

#ifdef DEBUG_SNPRINTF
  printf("zpad: %d, spad: %d, min: %d, max: %d, place: %d\n",
	 zpadlen, spadlen, min, max, place);
#endif

  /* Spaces */
  while (spadlen > 0) {
    dopr_outch (buffer, currlen, maxlen, ' ');
    --spadlen;
  }

  /* Sign */
  if (signvalue) 
    dopr_outch (buffer, currlen, maxlen, signvalue);

  /* Zeros */
  if (zpadlen > 0) {
    while (zpadlen > 0) {
      dopr_outch (buffer, currlen, maxlen, '0');
      --zpadlen;
    }
  }

  /* Digits */
  while (place > 0) 
    dopr_outch (buffer, currlen, maxlen, convert[--place]);

  /* Left Justified spaces */
  while (spadlen < 0) {
    dopr_outch (buffer, currlen, maxlen, ' ');
    ++spadlen;
  }
}

static LDOUBLE
abs_val(LDOUBLE value)
{
  LDOUBLE result = value;

  if (value < 0)
    result = -value;

  return result;
}

static LDOUBLE
POW10(int exp)
{
  LDOUBLE result = 1;

  while (exp) {
    result *= 10;
    exp--;
  }

  return result;
}

static LLONG
ROUND(LDOUBLE value)
{
  LLONG intpart;

  intpart = (LLONG)value;
  value = value - intpart;
  if (value >= 0.5)
    intpart++;

  return intpart;
}

/* a replacement for modf that doesn't need the math library. Should
   be portable, but slow */
static double
my_modf(double x0, double *iptr)
{
  int i;
  long l;
  double x = x0;
  double f = 1.0;

  for (i=0;i<100;i++) {
    l = (long)x;
    if (l <= (x+1) && l >= (x-1))
      break;
    x *= 0.1;
    f *= 10.0;
  }

  if (i == 100) {
    /* yikes! the number is beyond what we can handle. What do we do? */
    (*iptr) = 0;
    return 0;
  }

  if (i != 0) {
    double i2;
    double ret;

    ret = my_modf(x0-l*f, &i2);
    (*iptr) = l*f + i2;
    return ret;
  } 

  (*iptr) = l;
  return x - (*iptr);
}

static void
fmtfp (char *buffer, size_t *currlen, size_t maxlen,
       LDOUBLE fvalue, int min, int max, int flags)
{
  int signvalue = 0;
  double ufvalue;
  char iconvert[311];
  char fconvert[311];
  int iplace = 0;
  int fplace = 0;
  int padlen = 0; /* amount to pad */
  int zpadlen = 0; 
  int caps = 0;
  int index;
  double intpart;
  double fracpart;
  double temp;
  
  /* 
   * AIX manpage says the default is 0, but Solaris says the default
   * is 6, and sprintf on AIX defaults to 6
   */
  if (max < 0)
    max = 6;

  ufvalue = abs_val (fvalue);

  if (fvalue < 0) {
    signvalue = '-';
  } else {
    if (flags & DP_F_PLUS) { /* Do a sign (+/i) */
      signvalue = '+';
    } else {
      if (flags & DP_F_SPACE)
	signvalue = ' ';
    }
  }

#if 0
  if (flags & DP_F_UP)
    caps = 1; /* Should characters be upper case? */
#endif

#if 0
  if (max == 0)
    ufvalue += 0.5; /* if max = 0 we must round */
#endif

  /* 
   * Sorry, we only support 16 digits past the decimal because of our 
   * conversion method
   */
  if (max > 16)
    max = 16;

  /* We "cheat" by converting the fractional part to integer by
   * multiplying by a factor of 10
   */

  temp = ufvalue;
  my_modf(temp, &intpart);

  fracpart = ROUND((POW10(max)) * (ufvalue - intpart));

  if (fracpart >= POW10(max)) {
    intpart++;
    fracpart -= POW10(max);
  }

  /* Convert integer part */
  do {
    temp = intpart;
    my_modf(intpart*0.1, &intpart);
    temp = temp*0.1;
    index = (int) ((temp -intpart +0.05)* 10.0);
    /* index = (int) (((double)(temp*0.1) -intpart +0.05) *10.0); */
    /* printf ("%llf, %f, %x\n", temp, intpart, index); */
    iconvert[iplace++] =
      (caps? "0123456789ABCDEF":"0123456789abcdef")[index];
  } while (intpart && (iplace < 311));
  if (iplace == 311) iplace--;
  iconvert[iplace] = 0;

  /* Convert fractional part */
  if (fracpart)
    {
      do {
	temp = fracpart;
	my_modf(fracpart*0.1, &fracpart);
	temp = temp*0.1;
	index = (int) ((temp -fracpart +0.05)* 10.0);
	/* index = (int) ((((temp/10) -fracpart) +0.05) *10); */
	/* printf ("%lf, %lf, %ld\n", temp, fracpart, index); */
	fconvert[fplace++] =
	  (caps? "0123456789ABCDEF":"0123456789abcdef")[index];
      } while(fracpart && (fplace < 311));
      if (fplace == 311) fplace--;
    }
  fconvert[fplace] = 0;
  
  /* -1 for decimal point, another -1 if we are printing a sign */
  padlen = min - iplace - max - 1 - ((signvalue) ? 1 : 0); 
  zpadlen = max - fplace;
  if (zpadlen < 0) zpadlen = 0;
  if (padlen < 0) 
    padlen = 0;
  if (flags & DP_F_MINUS) 
    padlen = -padlen; /* Left Justifty */

  if ((flags & DP_F_ZERO) && (padlen > 0)) {
    if (signvalue) {
      dopr_outch (buffer, currlen, maxlen, signvalue);
      --padlen;
      signvalue = 0;
    }
    while (padlen > 0) {
      dopr_outch (buffer, currlen, maxlen, '0');
      --padlen;
    }
  }
  while (padlen > 0) {
    dopr_outch (buffer, currlen, maxlen, ' ');
    --padlen;
  }
  if (signvalue) 
    dopr_outch (buffer, currlen, maxlen, signvalue);

  while (iplace > 0) 
    dopr_outch (buffer, currlen, maxlen, iconvert[--iplace]);

#ifdef DEBUG_SNPRINTF
  printf("fmtfp: fplace=%d zpadlen=%d padlen=%d\n", fplace, zpadlen, padlen);
#endif

  /*
   * Decimal point.  This should probably use locale to find the correct
   * char to print out.
   */
  if (max > 0) {
    dopr_outch (buffer, currlen, maxlen, '.');

    while (zpadlen > 0) {
      dopr_outch (buffer, currlen, maxlen, '0');
      --zpadlen;
    }

    while (fplace > 0) 
      dopr_outch (buffer, currlen, maxlen, fconvert[--fplace]);
  }

  while (padlen < 0) {
    dopr_outch (buffer, currlen, maxlen, ' ');
    ++padlen;
  }
}

static void
dopr_outch(char *buffer, size_t *currlen, size_t maxlen, char c)
{
  if (*currlen < maxlen) {
    buffer[(*currlen)] = c;
  }
  (*currlen)++;
}

int
libmin_printf(char *fmt, ...)
{
  char buf[1024], *s;
  va_list ap;

  va_start(ap, fmt);
  dopr(buf, 1024, fmt, ap);
  /* make sure the output string is terminated */
  buf[1023] = '\0';
  va_end(ap);

  int cnt = 0;
  for (s=buf; *s; s++)
  {
    libtarg_putc(*s);
    cnt++;
  }
  return cnt;
}

int
libmin_snprintf(char *s, size_t size, char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  dopr(s, size, fmt, ap);
  /* make sure the output string is terminated */
  s[size-1] = '\0';
  va_end(ap);
  return libmin_strlen(s);
}

