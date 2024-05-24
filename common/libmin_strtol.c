#include "libmin.h"
#include "libtarg.h"

/*
 * This is a simple implementation of Standard C strtol().  A library
 * version should be programmed with more care.
 */
#define IN(l,a,r) (((l) <= (a)) && ((a) <= (r)))
long
libmin_strtol(const char  *nptr, char** endptr, int base)
{
  int			c;		/* current character value */
  int			digit;		/* digit value */
  static const char *digits = "0123456789abcdefghijklmnopqrstuvxwyz";
  int		is_negative;	/* false for positive, true for negative */
  long		number;		/* the accumulating number */
  const char		*pos;		/* pointer into digit list */
  const char		*q;		/* pointer past end of digits */

  if (!(IN(2,base,36) || (base == 0) || (nptr != (const char*)NULL)))
  {
    if (endptr != (char**)NULL)
	    *endptr = (char*)nptr;
    return (0L);
  }

  while (isspace((int)*nptr))
	  nptr++;				/* ignore leading whitespace */

  switch (*nptr)			/* set number sign */
  {
  case '-':
    is_negative = TRUE; 
    nptr++;
    break;

  case '+':
    is_negative = FALSE;
    nptr++;
    break;

  default:
    is_negative = FALSE;
    break;
  }

  q = nptr;
  if (base == 0)			/* variable base; set by lookahead */
  {
	  if (*q == '0')
	    base = ((*(q+1) == 'x') || (*(q+1) == 'X')) ? 16 : 8;
	else
	    base = 10;
  }

  /* eliminate optional "0x" or "0X" prefix */
  if ((base == 16) && (*q == '0') && ((*(q+1) == 'x') || (*(q+1) == 'X')) )
	  q += 2;

  number = 0L;

  /* Number conversion is done by shifting rather than multiplication
     when the base is a power of 2, in order that the results not be
     impacted by integer overflow. */
  switch (base)
  {
  case 2:
  	while (IN('0',*q,'1'))
  	{
	    number <<= 1;
	    number |= *q - '0';
	    q++;
  	}
    break;

  case 4:
  	while (IN('0',*q,'3'))
    {
	    number <<= 2;
	    number |= *q - '0';
	    q++;
	  }
	  break;

  case 8:
	  while (IN('0',*q,'7'))
	  {
	    number <<= 3;
	    number |= *q - '0';
	    q++;
	  }
	  break;


  case 16:
	  for (;;)
	  {
	    if (*q == '\0')
		    break;
	    c = (int)(unsigned)*q;
	    if (isupper(c))
		    c = tolower(c);
	    pos = libmin_strchr(digits,c);
	    if (pos == (char*)NULL)
		    break;
	    digit = (int)(pos - digits);
	    if (!IN(0,digit,15))
		    break;
	    number <<= 4;
	    number |= digit;
	    q++;
	  }
	  break;

    default:		/* all other bases done by multiplication */
	  for (;;)	/* accumulate negative so most negative */
	  {		/* number on two's-complement is handled */
	    if (*q == '\0')
		    break;
	    c = (int)(unsigned)*q;
	    if (isupper(c))
		    c = tolower(c);
	    pos = libmin_strchr(digits,c);
	    if (pos == (char*)NULL)
		    break;
	    digit = (int)(pos - digits);
	    if (!IN(0,digit,base-1))
		    break;
	    number *= base;
	    number -= digit;
	    q++;
	  }
	  if (endptr != (char**)NULL)
	    *endptr = (char*)q;
	  if (is_negative)
	    return(number);
	  number = -number;
	  break;
  }
  if (is_negative)
	  number = -number;
  if (endptr != (char**)NULL)
	  *endptr = (char*)q;
  return (number);
}
