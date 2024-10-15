
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=spirograph.o

PROG=spirograph

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO spirograph.out
	rm -f FOO

