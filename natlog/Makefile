
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=natlog.o

PROG=natlog

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO natlog.$(TARGET_REFEXT)
	rm -f FOO

