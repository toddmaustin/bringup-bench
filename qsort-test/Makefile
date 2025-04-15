
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=qsort-test.o

PROG=qsort-test

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO qsort-test.$(TARGET_REFEXT)
	rm -f FOO

