
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=quaternions.o

PROG=quaternions

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO quaternions.out
	rm -f FOO

