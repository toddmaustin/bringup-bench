
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=kepler.o

PROG=kepler

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO kepler.out
	rm -f FOO

