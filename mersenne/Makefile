
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=mersenne.o

PROG=mersenne

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO mersenne.out
	rm -f FOO

