#
#  Copyright 2002-2003, BitRaker Inc.  All Rights Reserved.
#
#  THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF BITRAKER INC.
#  The copyright notice above does not evidence any actual or intended
#  publication of such source code.
#
#  The copyright of this software is the property of BitRaker Inc.  and
#  is to be treated as BitRaker Inc. Confidential.  It shall not be
#  copied, used, duplicated, demonstrated, licensed, or disclosed to
#  others in whole or part for any purpose without the prior, written
#  permission of BitRaker Inc.
#

TARG	= x86
CC	= gcc
GFLAGS	= -g -Wall -DTARG_X86 -DUSE_UNISTD -DLITTLE_ENDIAN -DSPEC_CPU2000_LINUX_I386 $(CFLAGS)
SIM	=
MV	= mv -f
DIFF	= diff -s
#DIFF	= diff -s --strip-trailing-cr

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -static -m32 -o $(PROG) $(OBJS) $(LIBS)

.SUFFIXES: .o .c

.c.o:
	$(CC) -m32 $(GFLAGS) -o $@ -c $<

build: $(PROG)

size: $(PROG)
	size $(PROG)

clean:
	-rm -f $(PROG) $(OBJS) $(TMPS) core core.* FOO *.out *~

clean-build:
	-rm -f $(OBJS) $(TMPS) core core.* FOO *.out *~

clean-anvil:
	-rm -f $(PROG).axf.bre $(PROG).axf.cfg core core.*

