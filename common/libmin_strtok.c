#include "libmin.h"
#include "libtarg.h"

char *
libmin_strtok(char *s, const char *sep)
{
	static char *p;
	if (!s && !(s = p))
    return NULL;
	s += libmin_strspn(s, sep);
	if (!*s)
    return p = 0;
	p = s + libmin_strcspn(s, sep);
	if (*p)
    *p++ = 0;
	else
    p = 0;
	return s;
}
