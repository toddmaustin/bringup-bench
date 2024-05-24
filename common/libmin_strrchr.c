#include "libmin.h"
#include "libtarg.h"

char *
libmin_strrchr(const char *s, int c)
{
  char *rtnval = 0;

  do {
    if (*s == c)
      rtnval = (char*)s;
  } while (*s++);
  return rtnval;
}
