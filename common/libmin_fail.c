#include "libmin.h"
#include "libtarg.h"

NORETURN void
libmin_fail(int code)
{
  libmin_printf("ERROR: failure with termination code `%d'\n", code);
  libtarg_fail(code);
}
