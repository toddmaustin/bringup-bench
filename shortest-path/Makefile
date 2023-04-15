
LOCAL_CFLAGS= -I../common
LOCAL_LIBS=

PROG=shortest-path
OBJS=shortest-path.o ../common/utils.o

include ../config.mk
include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) $(TARGET_EXE) > FOO
	$(TARGET_DIFF) FOO shortest-path.out
	rm -f FOO

