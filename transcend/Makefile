
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=transcend.o

PROG=transcend

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO transcend.$(TARGET_REFEXT)
	rm -f FOO

