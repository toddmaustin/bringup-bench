#include "libmin.h"
#include "libtarg.h"

/* duplicate S, returning an identical malloc'd string */
char *
libmin_strdup (const char *s)
{
  size_t len = libmin_strlen(s) + 1;
  void *new = libmin_malloc(len);

  if (new == NULL)
    return NULL;

  return (char *)libmin_memcpy(new, s, len);
}
