
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=c-interp.o

PROG=c-interp

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) hello.c > FOO 
	$(TARGET_DIFF) FOO c-interp.out
	rm -f FOO

