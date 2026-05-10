LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=randshell-sort.o

PROG=randshell-sort

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO randshell-sort.$(TARGET_REFEXT)
	rm -f FOO
