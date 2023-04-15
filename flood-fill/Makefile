
LOCAL_CFLAGS= -I../common
LOCAL_LIBS=

PROG=flood-fill
OBJS=flood-fill.o ../common/utils.o

include ../config.mk
include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) $(TARGET_EXE) > FOO
	$(TARGET_DIFF) FOO flood-fill.out
	rm -f FOO

