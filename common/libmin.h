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
size_t libmin_strspn(const char *s, const char *c);
char *libmin_strtok(char *s, const char *sep);
char *libmin_strdup (const char *s);
char *libmin_strchr(const char *s, char c);
char *libmin_strrchr(const char *s, int c);
size_t libmin_strcspn(const char *s, const char *c);
char * libmin_strpbrk(const char *s, const char *b);

/* set a block of memory to a value */
void *libmin_memset(void *dest, int c, size_t n);
void *libmin_memcpy(void *dest, const void *src, size_t n);
int libmin_memcmp(const void *vl, const void *vr, size_t n);
void *libmin_memmove(void *dest, const void *src, size_t n);


#ifndef LIBTARG_SILENT
/* print a message with format FMT to the co-simulation console */
int libmin_printf(char *fmt, ...);
#else /* LIBTARG_SILENT */
/* run silent */
#define libmin_printf(FMT, ARGS...)	do { ; } while (0)
#endif /* LIBTARG_SILENT */

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

/* largest random number */
#define RAND_MAX (0x7fffffff)

/* largest numbers */
#define UINT32_MAX (0xFFFFFFFFU)

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

#define NULL	((void *)0)

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

#ifdef notdef
int isspace(int c);
int isupper(int c);
int islower(int c);
int isdigit(int c);
int isxdigit(int c);
int ispunct(int c);
int isalpha(int c);
int isalnum(int c);
int isprint(int c);
int isgraph(int c);
int iscntrl(int c);
int isleadbyte(int c);

int toupper(int c);
int tolower(int c);
#endif

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

#define DBL_EPSILON 2.22044604925031308085e-16

double libmin_floor(double x);
double libmin_scalbn(double x, int n);
double libmin_cos(double x);
double libmin_sin(double x);
double fabs(double x);
double libmin_pow(double x, double y);
double libmin_sqrt(double x);

int libmin_abs(int i);

/* libmin assertions */
#define libmin_assert(P)    ((P) ? (void)0 : (void)libmin_fail(1))

#endif /* LIBMIN_H */
