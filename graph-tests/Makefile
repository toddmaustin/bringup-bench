
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=graph-tests.o

PROG=graph-tests

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO graph-tests.out
	rm -f FOO

