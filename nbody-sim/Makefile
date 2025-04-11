
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=nbody-sim.o

PROG=nbody-sim

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO nbody-sim.out
	rm -f FOO

