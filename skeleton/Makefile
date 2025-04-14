
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=skeleton.o

PROG=skeleton

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO skeleton.$(TARGET_REFEXT)
	rm -f FOO

