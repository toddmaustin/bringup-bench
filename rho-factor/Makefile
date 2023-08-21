
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=rho-factor.o

PROG=rho-factor

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO rho-factor.out
	rm -f FOO

