
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=strange.o

PROG=strange

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO strange.out
	rm -f FOO

