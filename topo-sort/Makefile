
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=topo-sort.o

PROG=topo-sort

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO topo-sort.out
	rm -f FOO

