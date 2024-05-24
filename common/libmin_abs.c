#include "libmin.h"
#include "libtarg.h"

/* return the absolute value of I  */
int
libmin_abs(int i)
{
  return i < 0 ? -i : i;
}
