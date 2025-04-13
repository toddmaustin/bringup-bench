
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=lu-decomp.o

PROG=lu-decomp

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO lu-decomp.$(TARGET_REFEXT)
	rm -f FOO

