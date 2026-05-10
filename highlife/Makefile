LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=highlife.o

PROG=highlife

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO
	$(TARGET_DIFF) FOO highlife.$(TARGET_REFEXT)
	rm -f FOO
