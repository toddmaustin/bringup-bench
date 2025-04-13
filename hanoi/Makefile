
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=hanoi.o

PROG=hanoi

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO hanoi.$(TARGET_REFEXT)
	rm -f FOO

