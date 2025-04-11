
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=bit-kernels.o

PROG=bit-kernels

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO bit-kernels.out
	rm -f FOO

