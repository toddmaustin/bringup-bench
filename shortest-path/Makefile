
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=shortest-path.o

PROG=shortest-path

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO shortest-path.out
	rm -f FOO

