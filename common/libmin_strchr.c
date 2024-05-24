#include "libmin.h"
#include "libtarg.h"

char *
libmin_strchr(const char *s, char c)
{
  do
  {
    if (*s == c)
	    return (char*)s;
  } while (*s++);
  return NULL;
}
