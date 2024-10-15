
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=k-means.o

PROG=k-means

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO k-means.out
	rm -f FOO

