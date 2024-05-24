#include "libmin.h"
#include "libtarg.h"

int
libmin_strcmp(const char *l, const char *r)
{
	for (; *l==*r && *l && *r; l++, r++);
	return *(unsigned char *)l - *(unsigned char *)r;
}
