
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=rand-test.o

PROG=rand-test

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO rand-test.$(TARGET_REFEXT)
	rm -f FOO

