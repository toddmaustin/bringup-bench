
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=knapsack.o

PROG=knapsack

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO knapsack.$(TARGET_REFEXT)
	rm -f FOO

