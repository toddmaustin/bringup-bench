
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=blake2b.o

PROG=blake2b

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO blake2b.out
	rm -f FOO

