#ifndef LIBTARG_H
#define LIBTARG_H

/* basic numerics and data types */

#if '\xff' > 0
#define CHAR_MIN 0
#define CHAR_MAX 255
#else
#define CHAR_MIN (-128)
#define CHAR_MAX 127
#endif

#define CHAR_BIT 8
#define SCHAR_MIN (-128)
#define SCHAR_MAX 127
#define UCHAR_MAX 255
#define SHRT_MIN  (-1-0x7fff)
#define SHRT_MAX  0x7fff
#define USHRT_MAX 0xffff
#define INT_MIN  (-1-0x7fffffff)
#define INT_MAX  0x7fffffff
#define UINT_MAX 0xffffffffU
#define LONG_MIN (-LONG_MAX-1)
#define LONG_MAX __LONG_MAX
#define ULONG_MAX (2UL*LONG_MAX+1)
#define LLONG_MIN (-LLONG_MAX-1)
#define LLONG_MAX  0x7fffffffffffffffLL
#define ULLONG_MAX (2ULL*LLONG_MAX+1)

/* size type */
typedef unsigned long         size_t;

/* signed */
typedef signed char           int8_t;
typedef short int             int16_t;
typedef int                   int32_t;
typedef long int              int64_t;

/* unsigned */
typedef unsigned char         uint8_t;
typedef unsigned short int    uint16_t;
typedef unsigned int          uint32_t;
typedef unsigned long int     uint64_t;

/* floating point */
typedef float                 float_t;
typedef double                double_t;

/* pointer as an uint type */
typedef uint64_t              uintptr_t;

/* benchmark completed successfully */
void libtarg_success(void);

/* benchmark completed with error CODE */
void libtarg_fail(int code);

/* output a single character, to whereever the target wants to send it... */
void libtarg_putc(char c);

/* set memory breakpoint */
int libtarg_brk(void *addr);

/* get some memory */
void *libtarg_sbrk(size_t inc);

/* define support for vararg functions */
#ifdef TARGET_HOST
#include <stdarg.h>
#else
#error Error: no definition for vararg functions!
#endif

#endif /* LIBTARG_H */
