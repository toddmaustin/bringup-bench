#include "libmin.h"
#include "libtarg.h"

char *
libmin_strncat(char *d, const char *s, size_t n)
{
	char *a = d;
	d += libmin_strlen(d);
	while (n && *s)
    n--, *d++ = *s++;
	*d++ = 0;
	return a;
}
