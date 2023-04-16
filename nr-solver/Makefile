
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=nr-solver.o

PROG=nr-solver

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO nr-solver.out
	rm -f FOO

