
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=gcd-list.o

PROG=gcd-list

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO gcd-list.out
	rm -f FOO

