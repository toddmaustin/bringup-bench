
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=distinctness.o

PROG=distinctness

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO distinctness.$(TARGET_REFEXT)
	rm -f FOO

