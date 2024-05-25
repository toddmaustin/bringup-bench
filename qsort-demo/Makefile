
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=qsort-demo.o

PROG=qsort-demo

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO qsort-demo.out
	rm -f FOO

