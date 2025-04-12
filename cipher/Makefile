
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=cipher.o

PROG=cipher

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO cipher.$(TARGET_REFEXT)
	rm -f FOO

