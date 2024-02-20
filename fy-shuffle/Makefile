
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=fy-shuffle.o

PROG=fy-shuffle

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO fy-shuffle.out
	rm -f FOO

