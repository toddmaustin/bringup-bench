
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=uniquify.o

PROG=uniquify

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO uniquify.$(TARGET_REFEXT)
	rm -f FOO

