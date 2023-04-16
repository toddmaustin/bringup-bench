
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=hanoi.o

PROG=hanoi

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO hanoi.out
	rm -f FOO

