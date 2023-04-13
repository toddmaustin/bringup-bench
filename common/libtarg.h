#ifndef LIBTARG_H
#define LIBTARG_H

typedef unsigned long size_t;

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

/* benchmark completed successfully */
void libtarg_success(void);

/* benchmark completed with error CODE */
void libtarg_fail(int code);

/* output a single character, to whereever the target wants to send it... */
void libtarg_putc(char c);

#endif /* LIBTARG_H */
