
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=tiny-NN.o

PROG=tiny-NN

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO tiny-NN.$(TARGET_REFEXT)
	rm -f FOO

