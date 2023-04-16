
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=parrondo.o

PROG=parrondo

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO parrondo.out
	rm -f FOO

