
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=convex-hull.o

PROG=convex-hull

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO convex-hull.$(TARGET_REFEXT)
	rm -f FOO

