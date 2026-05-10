LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=partition-equal.o

PROG=partition-equal

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO
	$(TARGET_DIFF) FOO partition-equal.$(TARGET_REFEXT)
	rm -f FOO
