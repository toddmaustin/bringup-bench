
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=huff-encode.o

PROG=huff-encode

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO huff-encode.out
	rm -f FOO

