
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=rsa-cipher.o

PROG=rsa-cipher

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO rsa-cipher.$(TARGET_REFEXT)
	rm -f FOO

