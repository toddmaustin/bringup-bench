
LOCAL_CFLAGS=
LOCAL_LIBS=

PROG=nr-solver
OBJS=nr-solver.o ../common/utils.o

include ../config.mk
include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) $(TARGET_EXE) > FOO
	$(TARGET_DIFF) FOO nr-solver.out
	rm -f FOO

