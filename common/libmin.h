#ifndef LIBMIN_H
#define LIBMIN_H

#include "libtarg.h"

#define TRUE  1
#define FALSE 0

/* Build parameters:

   #define LIBMIN_SILENT		- disable all console messages
   #define LIBMIN_HOST		- build to run on Unix host
   #define LIBMIN_TARGET		- build to run on ARM target model

*/

/* standard atol() implementation */
long libmin_atol(const char *s);

/* standard atoi/atof implementation */
int libmin_atoi(const char *s);
double libmin_atof(const char *s);
long libmin_strtol(const char *s, char **endptr, int base);

/* getopt() hooks */
extern char *optarg;
extern int optind, opterr, optopt, optpos, optreset;

/* standard getopt() implementation */
int libmin_getopt(int argc, char * const argv[], const char *optstring);

/* copy src to dst, truncating or null-padding to always copy n bytes */
char *libmin_strcat(char *dest, const char *src);
char *libmin_strcpy(char *dest, const char *src);
char *libmin_strncpy(char *dst, const char *src, size_t n);
char *libmin_strncat(char *d, const char *s, size_t n);

/* return string length */
size_t libmin_strlen(const char *str);

/* return order of strings */
int libmin_strcmp(const char *l, const char *r);
int libmin_strncmp(const char *s1, const char *s2, register size_t n);
size_t libmin_strspn(const char *s, const char *c);
char *libmin_strtok(char *s, const char *sep);
char *libmin_strdup (const char *s);
char *libmin_strchr(const char *s, char c);
char *libmin_strrchr(const char *s, int c);
size_t libmin_strcspn(const char *s, const char *c);
char *libmin_strpbrk(const char *s, const char *b);
const char *libmin_strstr (const char *s1, const char *s2);
char *libmin_strcasestr(const char *h, const char *n);
int libmin_strncasecmp(const char *_l, const char *_r, size_t n);

/* set a block of memory to a value */
void *libmin_memset(void *dest, int c, size_t n);
void *libmin_memcpy(void *dest, const void *src, size_t n);
int libmin_memcmp(const void *vl, const void *vr, size_t n);
void *libmin_memmove(void *dest, const void *src, size_t n);


#ifndef TARGET_SILENT
/* print a message with format FMT to the co-simulation console */
int libmin_printf(char *fmt, ...);
int libmin_snprintf(char *s, size_t size, char *fmt, ...);
#else /* TARGET_SILENT */
/* run silent */
#define libmin_printf(FMT, ARGS...)	do { ; } while (0)
#endif /* TARGET_SILENT */

/* print one character */
void libmin_putc(char c);

/* print one string */
void libmin_puts(char *s);

/* scan a string */
int libmin_sscanf(const char *buf, const char *fmt, ...);

/* failure/success codes */
#define EXIT_FAILURE  1 /* failing exit status */
#define EXIT_SUCCESS  0 /* successful exit status */

/* successfully exit co-simulation */
void libmin_success(void);

/* exit co-simulation with failure exit code CODE */
void libmin_fail(int code);

/* largest random number, must be power-of-two-minus-one! */
#define RAND_MAX (0x7fffffff)

#ifdef notdef
/* largest numbers */
#define UINT32_MAX (0xFFFFFFFFU)
#endif /* notdef */

/* see the random integer generator */
void libmin_srand(unsigned int seed);

/* generate a random integer */
unsigned int libmin_rand(void);

/* allocate memory */
void *libmin_malloc(size_t size);
void *libmin_calloc(size_t m, size_t n);
void *libmin_realloc(void *block, size_t size);

/* free memory */
void libmin_free(void * addr);

#ifndef __clang__
#define NULL	((void *)0)
#endif

/* in-memory file I/O */
struct _MFILE {
  char *fname;
  size_t data_sz;
  const uint8_t *data;
  int rdptr;
};
typedef struct _MFILE MFILE;

#define EOF (-1)

/* open an in-memory file */
void libmin_mopen(MFILE *mfile, const char *mode);

/* return in-memory file size */
size_t libmin_msize(MFILE *mfile);

/* at end of file */
int libmin_meof(MFILE *mfile);

/* close the in-memory file */
void libmin_mclose(MFILE *mfile);

/* read a buffer from the in-memory file */
size_t libmin_mread(void *ptr, size_t size, MFILE *mfile);

/* get a string from the in-memory file */
char *libmin_mgets(char *s, size_t size, MFILE *mfile);

/* get a line from the in-memory file */
int libmin_getline(char **s, size_t *n, MFILE *f);

/* read a character from the in-memory file */
int libmin_mgetc(MFILE *mfile);

/* sort an array */
typedef int (*cmpfun)(const void *, const void *);
void libmin_qsort(void *base, size_t nel, size_t width, cmpfun cmp);


/* ctype defs */
//
// Character types
//

#define _UPPER          0x1     // Upper case letter
#define _LOWER          0x2     // Lower case letter
#define _DIGIT          0x4     // Digit[0-9]
#define _SPACE          0x8     // Tab, carriage return, newline, vertical tab or form feed
#define _PUNCT          0x10    // Punctuation character
#define _CONTROL        0x20    // Control character
#define _BLANK          0x40    // Space char
#define _HEX            0x80    // Hexadecimal digit

#define _LEADBYTE       0x8000                      // Multibyte leadbyte
#define _ALPHA          (0x0100 | _UPPER| _LOWER)   // Alphabetic character

extern unsigned short *_pctype; // pointer to table for char's

int _isctype(int c, int mask);

#define isalpha(c)     (_pctype[(int)(c)] & (_UPPER | _LOWER))
#define isupper(c)     (_pctype[(int)(c)] & _UPPER)
#define islower(c)     (_pctype[(int)(c)] & _LOWER)
#define isdigit(c)     (_pctype[(int)(c)] & _DIGIT)
#define isxdigit(c)    (_pctype[(int)(c)] & _HEX)
#define isspace(c)     (_pctype[(int)(c)] & _SPACE)
#define ispunct(c)     (_pctype[(int)(c)] & _PUNCT)
#define isalnum(c)     (_pctype[(int)(c)] & (_UPPER | _LOWER | _DIGIT))
#define isprint(c)     (_pctype[(int)(c)] & (_BLANK | _PUNCT | _UPPER | _LOWER | _DIGIT))
#define isgraph(c)     (_pctype[(int)(c)] & (_PUNCT | _UPPER | _LOWER | _DIGIT))
#define iscntrl(c)     (_pctype[(int)(c)] & _CONTROL)
#define isleadbyte(c)  (_pctype[(int)(unsigned char)(c)] & _LEADBYTE)

#define tolower(c)     (isupper(c) ? ((c) - 'A' + 'a') : (c))
#define toupper(c)     (islower(c) ? ((c) - 'a' + 'A') : (c))

/* math functions */

#define FORCE_EVAL(x) do {                        \
	if (sizeof(x) == sizeof(float)) {         \
		volatile float __x;               \
		__x = (x); (void)__x;                        \
	} else if (sizeof(x) == sizeof(double)) { \
		volatile double __x;              \
		__x = (x); (void)__x;                        \
	} else {                                  \
		volatile long double __x;         \
		__x = (x); (void)__x;                        \
	}                                         \
} while(0)

/* Get two 32 bit ints from a double.  */
#define EXTRACT_WORDS(hi,lo,d)                    \
do {                                              \
  union {double f; uint64_t i;} __u;              \
  __u.f = (d);                                    \
  (hi) = __u.i >> 32;                             \
  (lo) = (uint32_t)__u.i;                         \
} while (0)

/* Get the more significant 32 bit int from a double.  */
#define GET_HIGH_WORD(hi,d)                       \
do {                                              \
  union {double f; uint64_t i;} __u;              \
  __u.f = (d);                                    \
  (hi) = __u.i >> 32;                             \
} while (0)

/* Get the less significant 32 bit int from a double.  */
#define GET_LOW_WORD(lo,d)                        \
do {                                              \
  union {double f; uint64_t i;} __u;              \
  __u.f = (d);                                    \
  (lo) = (uint32_t)__u.i;                         \
} while (0)

/* Set a double from two 32 bit ints.  */
#define INSERT_WORDS(d,hi,lo)                     \
do {                                              \
  union {double f; uint64_t i;} __u;              \
  __u.i = ((uint64_t)(hi)<<32) | (uint32_t)(lo);  \
  (d) = __u.f;                                    \
} while (0)

/* Set the more significant 32 bits of a double from an int.  */
#define SET_HIGH_WORD(d,hi)                       \
do {                                              \
  union {double f; uint64_t i;} __u;              \
  __u.f = (d);                                    \
  __u.i &= 0xffffffff;                            \
  __u.i |= (uint64_t)(hi) << 32;                  \
  (d) = __u.f;                                    \
} while (0)

/* Set the less significant 32 bits of a double from an int.  */
#define SET_LOW_WORD(d,lo)                        \
do {                                              \
  union {double f; uint64_t i;} __u;              \
  __u.f = (d);                                    \
  __u.i &= 0xffffffff00000000ull;                 \
  __u.i |= (uint32_t)(lo);                        \
  (d) = __u.f;                                    \
} while (0)

/* Get a 32 bit int from a float.  */
#define GET_FLOAT_WORD(w,d)                       \
do {                                              \
  union {float f; uint32_t i;} __u;               \
  __u.f = (d);                                    \
  (w) = __u.i;                                    \
} while (0)

/* Set a float from a 32 bit int.  */
#define SET_FLOAT_WORD(d,w)                       \
do {                                              \
  union {float f; uint32_t i;} __u;               \
  __u.i = (w);                                    \
  (d) = __u.f;                                    \
} while (0)

#undef __CMPLX
#undef CMPLX
#undef CMPLXF
#undef CMPLXL

#define __CMPLX(x, y, t) \
	((union { _Complex t __z; t __xy[2]; }){.__xy = {(x),(y)}}.__z)

#define CMPLX(x, y) __CMPLX(x, y, double)
#define CMPLXF(x, y) __CMPLX(x, y, float)
#define CMPLXL(x, y) __CMPLX(x, y, long double)

#if FLT_EVAL_METHOD==0 || FLT_EVAL_METHOD==1
#define EPS DBL_EPSILON
#elif FLT_EVAL_METHOD==2
#define EPS LDBL_EPSILON
#endif

#define DBL_EPSILON 2.22044604925031308085e-16

#define FP_NAN       0
#define FP_INFINITE  1
#define FP_ZERO      2
#define FP_SUBNORMAL 3
#define FP_NORMAL    4

static __inline unsigned __FLOAT_BITS(float __f)
{
	union {float __f; unsigned __i;} __u;
	__u.__f = __f;
	return __u.__i;
}
static __inline unsigned long long __DOUBLE_BITS(double __f)
{
	union {double __f; unsigned long long __i;} __u;
	__u.__f = __f;
	return __u.__i;
}

static __inline int __fpclassify(double x)
{
	union {double f; uint64_t i;} u = {x};
	int e = u.i>>52 & 0x7ff;
	if (!e) return u.i<<1 ? FP_SUBNORMAL : FP_ZERO;
	if (e==0x7ff) return u.i<<12 ? FP_NAN : FP_INFINITE;
	return FP_NORMAL;
}

#define libmin_isfinite(x) ( \
	sizeof(x) == sizeof(float) ? (__FLOAT_BITS(x) & 0x7fffffff) < 0x7f800000 : \
	sizeof(x) == sizeof(double) ? (__DOUBLE_BITS(x) & -1ULL>>1) < 0x7ffULL<<52 : \
	__fpclassify(x) > FP_INFINITE)

#define libmin_isnan(x) ( \
	sizeof(x) == sizeof(float) ? (__FLOAT_BITS(x) & 0x7fffffff) > 0x7f800000 : \
	sizeof(x) == sizeof(double) ? (__DOUBLE_BITS(x) & (uint64_t)-1>>1) > (uint64_t)0x7ff<<52 : \
	__fpclassify(x) == FP_NAN)

#define M_PI		3.14159265358979323846	/* pi */
#define NAN     (__builtin_nanf(""))


double libmin_floor(double x);
double libmin_scalbn(double x, int n);
double libmin_cos(double x);
double libmin_sin(double x);
double libmin_fabs(double x);
float libmin_fabsf(float x);
double libmin_pow(double x, double y);
double libmin_sqrt(double x);
double libmin_exp(double x);
int libmin_abs(int i);
double libmin_acos(double x);
double libmin_asin(double x);
double libmin_atan2(double y, double x);
double libmin_atan(double x);

#define libmin_fmax(a,b)   (((a) > (b)) ? (a) : (b))

/* internal mathlib interfaces */
int __rem_pio2_large(double *x, double *y, int e0, int nx, int prec);
int __rem_pio2(double x, double *y);

/* libmin assertions */
#define libmin_assert(P)    ((P) ? (void)0 : (void)libmin_fail(1))

/* MIN/MAX functions */
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

#endif /* LIBMIN_H */
