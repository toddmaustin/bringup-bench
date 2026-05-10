
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=tea-cipher.o

PROG=tea-cipher

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO
	$(TARGET_DIFF) FOO tea-cipher.$(TARGET_REFEXT)
	rm -f FOO
