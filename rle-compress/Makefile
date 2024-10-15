
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=rle-compress.o

PROG=rle-compress

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO rle-compress.out
	rm -f FOO

