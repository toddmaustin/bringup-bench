
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=heapsort.o

PROG=heapsort

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO heapsort.out
	rm -f FOO

