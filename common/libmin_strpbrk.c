#include "libmin.h"
#include "libtarg.h"

char *
libmin_strpbrk(const char *s, const char *b)
{
	s += libmin_strcspn(s, b);
	return *s ? (char *)s : 0;
}
