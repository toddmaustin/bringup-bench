
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=murmur-hash.o

PROG=murmur-hash

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO murmur-hash.out
	rm -f FOO

