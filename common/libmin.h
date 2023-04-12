#ifndef LIBMIN_H
#define LIBMIN_H

/* Build parameters:

   #define LIBMIN_SILENT		- disable all console messages
   #define LIBMIN_HOST		- build to run on Unix host
   #define LIBMIN_TARGET		- build to run on ARM target model

*/

typedef unsigned long size_t;

/* return string length */
size_t libmin_strlen(const char *str);

#ifndef COSIM_SILENT
/* print a message with format FMT to the co-simulation console */
void libmin_printf(char *fmt, ...);
#else /* COSIM_SILENT */
/* run silent */
#define libmin_printf(FMT, ARGS...)	do { ; } while (0)
#endif /* COSIM_SILENT */

/* successfully exit co-simulation */
void libmin_success(void);

/* exit co-simulation with failure exit code CODE */
void libmin_fail(int code);

/* see the random integer generator */
void libmin_srand(unsigned int seed);

/* generate a random integer */
int libmin_rand(void);

#ifndef NULL
#define NULL	((void *)0)
#endif /* NULL */

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

#define isalpha(c)     (_pctype[(c)] & (_UPPER | _LOWER))
#define isupper(c)     (_pctype[(c)] & _UPPER)
#define islower(c)     (_pctype[(c)] & _LOWER)
#define isdigit(c)     (_pctype[(c)] & _DIGIT)
#define isxdigit(c)    (_pctype[(c)] & _HEX)
#define isspace(c)     (_pctype[(c)] & _SPACE)
#define ispunct(c)     (_pctype[(c)] & _PUNCT)
#define isalnum(c)     (_pctype[(c)] & (_UPPER | _LOWER | _DIGIT))
#define isprint(c)     (_pctype[(c)] & (_BLANK | _PUNCT | _UPPER | _LOWER | _DIGIT))
#define isgraph(c)     (_pctype[(c)] & (_PUNCT | _UPPER | _LOWER | _DIGIT))
#define iscntrl(c)     (_pctype[(c)] & _CONTROL)
#define isleadbyte(c)  (_pctype[(unsigned char)(c)] & _LEADBYTE)

#define tolower(c)     (isupper(c) ? ((c) - 'A' + 'a') : (c))
#define toupper(c)     (islower(c) ? ((c) - 'a' + 'A') : (c))

#endif /* LIBMIN_H */
