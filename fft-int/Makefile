
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=fft-int.o

PROG=fft-int

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO fft-int.out
	rm -f FOO

