#include "libmin.h"
#include "libtarg.h"

void
libmin_puts(char *s)
{
  for (; *s; s++)
    libtarg_putc(*s);
  libtarg_putc('\n');
}
