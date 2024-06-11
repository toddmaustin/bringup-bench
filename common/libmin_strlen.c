#include "libmin.h"
#include "libtarg.h"

size_t
libmin_strlen(const char *str)
{
  if (!str)
    return 0;
  
  const char *ptr = str;
  while (*str)
    ++str;

  return str - ptr;
}
