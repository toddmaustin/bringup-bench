#include "libmin.h"
#include "libtarg.h"

const char *
libmin_strstr(const char *s1, const char *s2)
{
  const char *p = s1;
  const size_t len = libmin_strlen(s2);

  if (!len)
    return s1;

  for (; (p = libmin_strchr(p, *s2)) != 0; p++)
    {
      if (libmin_strncmp (p, s2, len) == 0)
	      return (char *)p;
    }
  return NULL;
}
