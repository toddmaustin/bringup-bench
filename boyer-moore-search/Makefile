
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=boyer-moore-search.o

PROG=boyer-moore-search

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO boyer-moore-search.out
	rm -f FOO

