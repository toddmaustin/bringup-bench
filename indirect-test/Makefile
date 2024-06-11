
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=indirect-test.o

PROG=indirect-test

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO indirect-test.out
	rm -f FOO

