
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=pascal.o

PROG=pascal

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO pascal.out
	rm -f FOO

