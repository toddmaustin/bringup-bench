
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=tetris-sim.o

PROG=tetris-sim

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO tetris-sim.$(TARGET_REFEXT)
	rm -f FOO

