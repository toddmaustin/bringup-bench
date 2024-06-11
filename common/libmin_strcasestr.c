#include "libmin.h"

char *
libmin_strcasestr(const char *h, const char *n)
{
	size_t l = libmin_strlen(n);
	for (; *h; h++) if (!libmin_strncasecmp(h, n, l)) return (char *)h;
	return 0;
}
