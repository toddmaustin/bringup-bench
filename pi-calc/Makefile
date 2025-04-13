
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=pi-calc.o

PROG=pi-calc

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO pi-calc.$(TARGET_REFEXT)
	rm -f FOO

