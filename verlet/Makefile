
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=verlet.o

PROG=verlet

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO verlet.$(TARGET_REFEXT)
	rm -f FOO

