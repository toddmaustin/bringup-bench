
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=idct-alg.o

PROG=idct-alg

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO idct-alg.$(TARGET_REFEXT)
	rm -f FOO

