
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=heat-calc.o

PROG=heat-calc

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO heat-calc.$(TARGET_REFEXT)
	rm -f FOO

