#ifndef LIBTARG_H
#define LIBTARG_H

typedef unsigned long size_t;
typedef unsigned char uint8_t;

/* benchmark completed successfully */
void libtarg_success(void);

/* benchmark completed with error CODE */
void libtarg_fail(int code);

/* output a single character, to whereever the target wants to send it... */
void libtarg_cputc(char c);

#endif /* LIBTARG_H */
