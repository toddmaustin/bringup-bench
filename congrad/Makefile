
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=congrad.o

PROG=congrad

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO congrad.$(TARGET_REFEXT)
	rm -f FOO

