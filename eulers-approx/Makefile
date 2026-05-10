
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=eulers-approx.o

PROG=eulers-approx

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO eulers-approx.$(TARGET_REFEXT)
	rm -f FOO

