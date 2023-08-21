
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=quine.o

PROG=quine

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO quine.out
	rm -f FOO

