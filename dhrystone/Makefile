
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=dhrystone.o

PROG=dhrystone

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO dhrystone.out
	rm -f FOO

