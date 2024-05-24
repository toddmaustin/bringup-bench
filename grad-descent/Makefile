
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=grad-descent.o

PROG=grad-descent

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO grad-descent.out
	rm -f FOO

