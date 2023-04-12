/*
   Copyright 2002-2003, BitRaker Inc.  All Rights Reserved.

   THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF BITRAKER INC.
   The copyright notice above does not evidence any actual or intended
   publication of such source code.

   The copyright of this software is the property of BitRaker Inc.  and
   is to be treated as BitRaker Inc. Confidential.  It shall not be
   copied, used, duplicated, demonstrated, licensed, or disclosed to
   others in whole or part for any purpose without the prior, written
   permission of BitRaker Inc.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "adslib.h"

#ifdef TARG_ARM_ADS

unsigned int
read(int fd, void *buf, int len)
{
	unsigned  res;

	res = _sys_read(fd, (void *)(buf), len, OPEN_B);
	if (res == -1)
		return -1;
	else if (res == 0x80000000)
		return 0;
	else if ((res & 0x7fffffff) != 0)
		return len - (res & 0x7fffffff);
	/* else, res == 0 */
	return len;
}

int
lseek(int fd, long pos, int whence)
{
	long len;

	switch (whence)
	{
	case SEEK_SET:
		return (_sys_seek(fd, pos) >= 0) ? pos : -1;

	case SEEK_CUR:
		fprintf(stderr, "error: SEEK_CUR not supported\n");
		abort();

	case SEEK_END:
		len = _sys_flen(fd);
		if (_sys_seek(fd, len + pos) < 0)
			return -1;
		return len + pos;

	default:
		fprintf(stderr, "error: bad whence in lseek() = %d\n", whence);
		abort();
	}
	return 0;
}

/* strdup() - create a duplicate string */
char *
strdup(const char *s)
{
  char *p;
  unsigned int slen;
  slen = strlen(s);
  p = malloc(slen + 1);
  strcpy(p, s);
  return p;
}

#endif /* TARG_ARM_ADS */

