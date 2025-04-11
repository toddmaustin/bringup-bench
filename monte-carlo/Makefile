
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=monte-carlo.o

PROG=monte-carlo

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO monte-carlo.out
	rm -f FOO

