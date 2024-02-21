
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=simple-grep.o

PROG=simple-grep

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO simple-grep.out
	rm -f FOO

