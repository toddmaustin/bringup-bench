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

#ifndef ADSLIB_H
#define ADSLIB_H

#ifdef TARG_ARM_ADS

#include <stdio.h>
#include <stdlib.h>
#include <rt_sys.h>

#define EINVAL		22

/* close() interface, has same inputs, output semantics */
#define close(fd)               _sys_close(fd)

/* unlink() interface, has same inputs, output semantics */
#define unlink(filename)        (remove(filename))

/* write() interface, has same inputs, output semantics reflect POSIX */
#define write(fd,buf,len)       ((_sys_write(fd, (void *)(buf), len, OPEN_B) == 0) ? len : -1)

/* read() interface, has same inputs, output semantics reflect POSIX */
unsigned int read(int fd, void *buf, int len);

/* lseek() interface, has same inputs, output semantics reflect POSIX */
int lseek(int fd, long pos, int whence);

/* strdup() - create a duplicate string */
char *strdup(const char *s);

#endif /* TARG_ARM_ADS */

#endif /* ADSLIB_H */

