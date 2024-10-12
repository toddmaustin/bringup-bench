
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=priority-queue.o

PROG=priority-queue

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO priority-queue.out
	rm -f FOO

