
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=sieve.o

PROG=sieve

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO sieve.out
	rm -f FOO

