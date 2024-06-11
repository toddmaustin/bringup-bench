#include "libmin.h"
#include "libtarg.h"

void *
libmin_memmove(void *dest, const void *src, size_t n)
{
  size_t i;
  const unsigned char *usrc = src;
  unsigned char *udest = dest;

  if (udest < usrc)
  {
    for (i = 0; i < n; i++)
      udest[i] = usrc[i];
  }
  else if (udest > usrc)
  {
    for (i = n; i > 0; i--)
      udest[i-1] = usrc[i-1];
  }

  return dest;
}
