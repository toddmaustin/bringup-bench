
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=heldkarp-tsp.o

PROG=heldkarp-tsp

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO
	$(TARGET_DIFF) FOO heldkarp-tsp.$(TARGET_REFEXT)
	rm -f FOO
