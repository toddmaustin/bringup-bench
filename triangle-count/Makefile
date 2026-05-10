
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=triangle-count.o

PROG=triangle-count

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO triangle-count.$(TARGET_REFEXT)
	rm -f FOO

