#include "libmin.h"
#include "libtarg.h"

char *
libmin_strcpy(char *dest, const char *src)
{
	const char *s = src;
	char *d = dest;
	while ((*d++ = *s++));
	return dest;
}
