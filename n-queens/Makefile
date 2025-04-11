
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=n-queens.o

PROG=n-queens

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO n-queens.out
	rm -f FOO

