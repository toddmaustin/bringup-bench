
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=spelt2num.o

PROG=spelt2num

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO spelt2num.out
	rm -f FOO

