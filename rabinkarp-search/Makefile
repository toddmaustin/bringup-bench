
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=rabinkarp-search.o

PROG=rabinkarp-search

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO rabinkarp-search.out
	rm -f FOO

