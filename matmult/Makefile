
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=matmult.o

PROG=matmult

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO matmult.$(TARGET_REFEXT)
	rm -f FOO

