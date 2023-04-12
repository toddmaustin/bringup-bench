#include "libtarg.h"

#ifdef LIBTARG_HOST
#include <stdio.h>
#include <stdlib.h>
#else /* undefined target */
#error Co-simulation platform not defined, define LIBTARG_HOST or a target-dependent definition.
#endif

/* benchmark completed successfully */
void
libtarg_success(void)
{
#ifdef LIBTARG_HOST
  exit(0);
#endif
}

/* benchmark completed with error CODE */
void
libtarg_fail(int code)
{
#ifdef LIBTARG_HOST
  exit(code);
#endif
}

/* output a single character, to whereever the target wants to send it... */
void
libtarg_putc(char c)
{
#ifdef LIBTARG_HOST
  fputc(c, stdout);
#endif
}
