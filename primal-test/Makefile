
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=primal-test.o

PROG=primal-test

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO primal-test.out
	rm -f FOO

