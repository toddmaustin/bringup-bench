
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=c-interp.o

PROG=c-interp

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO c-interp.$(TARGET_REFEXT)
	rm -f FOO

