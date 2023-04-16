
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=flood-fill.o

PROG=flood-fill

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO flood-fill.out
	rm -f FOO

