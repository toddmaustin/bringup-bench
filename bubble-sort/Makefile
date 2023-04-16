
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=bubble-sort.o

PROG=bubble-sort

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO bubble-sort.out
	rm -f FOO

