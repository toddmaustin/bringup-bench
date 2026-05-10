LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=nonlinear-nn.o

PROG=nonlinear-nn

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO
	$(TARGET_DIFF) FOO nonlinear-nn.$(TARGET_REFEXT)
	rm -f FOO
