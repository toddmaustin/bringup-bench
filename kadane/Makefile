
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=kadane.o

PROG=kadane

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO kadane.out
	rm -f FOO

