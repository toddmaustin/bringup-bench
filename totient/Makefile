
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=totient.o

PROG=totient

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO totient.out
	rm -f FOO

