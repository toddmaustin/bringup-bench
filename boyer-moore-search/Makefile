
LOCAL_CFLAGS=
LOCAL_LIBS=

PROG=boyer-moore-search
OBJS=boyer-moore-search.o ../common/utils.o

include ../config.mk
include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) $(TARGET_EXE) > FOO
	-$(TARGET_DIFF) FOO boyer-moore-search.out
	rm -f FOO

