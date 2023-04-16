
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=longdiv.o

PROG=longdiv

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO longdiv.out
	rm -f FOO

