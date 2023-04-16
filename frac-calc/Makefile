
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=frac-calc.o

PROG=frac-calc

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO frac-calc.out
	rm -f FOO

