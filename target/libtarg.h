#ifndef LIBTARG_H
#define LIBTARG_H

/* define support for vararg functions */
#if defined(TARGET_HOST)
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#elif defined(TARGET_SA)
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#elif defined(TARGET_HAHOST)
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#elif defined(TARGET_SIMPLE) || defined(TARGET_SPIKE) || defined(TARGET_HASPIKE)
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#else
#error Error: no definition for vararg functions!
#endif

/* basic numerics and data types */

#if '\xff' > 0
#define CHAR_MIN 0
#define CHAR_MAX 255
#else
#define CHAR_MIN (-128)
#define CHAR_MAX 127
#endif

#define UCHAR_MAX   ((unsigned char)~0)
#define USHRT_MAX   ((unsigned short)~0)
#define UINT_MAX    (~0)
#define ULONG_MAX   (~0ul)
#define ULLONG_MAX  (~0ull)

#if __clang__

#define CHAR_BIT      __CHAR_BIT__
#define SCHAR_MAX     __SCHAR_MAX__
#define SHRT_MAX      __SHRT_MAX__
#define INT_MAX       __INT_MAX__
#define LONG_MAX      __LONG_MAX__
#define LLONG_MAX     __LONG_LONG_MAX__

/* size type */
typedef __SIZE_TYPE__   size_t;
typedef signed __SIZE_TYPE__   ssize_t;

/* signed */
typedef __INT8_TYPE__   int8_t;
typedef __INT16_TYPE__  int16_t;
typedef __INT32_TYPE__  int32_t;
typedef __INT64_TYPE__  int64_t;

/* unsigned */
typedef __UINT8_TYPE__  uint8_t;
typedef __UINT16_TYPE__ uint16_t;
typedef __UINT32_TYPE__ uint32_t;
typedef __UINT64_TYPE__ uint64_t;

/* pointer as an int type */
typedef __INTPTR_TYPE__  intptr_t;
typedef __UINTPTR_TYPE__ uintptr_t;

#else

#define CHAR_BIT 8
#define SCHAR_MAX 127
#define SHRT_MAX  0x7fff
#define INT_MAX  0x7fffffff
#define LONG_MAX 9223372036854775807L
#define LLONG_MAX  0x7fffffffffffffffLL

#ifdef notdef
/* size type */
typedef unsigned long         size_t;
typedef signed long           ssize_t;
#endif /* notdef */

#ifdef notdef
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

/* pointer as an int type */
typedef int64_t               intptr_t;
typedef uint64_t              uintptr_t;
#endif /* notdef */

#endif

#define SCHAR_MIN     (-SCHAR_MAX - 1)
#define SHRT_MIN      (-SHRT_MAX - 1)
#define INT_MIN       (-INT_MAX - 1)
#define LONG_MIN      (-LONG_MAX - 1)
#define LLONG_MIN     (-LLONG_MAX - 1)

#define DBL_MAX        1.7976931348623157e+308
#define DBL_MIN        2.2250738585072014e-308
#define FLT_MAX        3.4028234663852886e+38
#define FLT_MIN        1.1754943508222875e-38


/* floating point */
typedef float                 float_t;
typedef double                double_t;

/* benchmark completed successfully */
void libtarg_success(void);

/* benchmark completed with error CODE */
void libtarg_fail(int code);

/* output a single character, to whereever the target wants to send it... */
void libtarg_putc(char c);

/* get some memory */
void *libtarg_sbrk(size_t inc);

#endif /* LIBTARG_H */
