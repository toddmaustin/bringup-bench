
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=mojo-test.o

PROG=mojo-test

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO mojo-test.$(TARGET_REFEXT)
	rm -f FOO

