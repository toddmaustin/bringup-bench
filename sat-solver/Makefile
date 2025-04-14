
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=sat-solver.o

PROG=sat-solver

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO sat-solver.$(TARGET_REFEXT)
	rm -f FOO

