
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=mojov-test.o simon.o

PROG=mojov-test

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO mojov-test.$(TARGET_REFEXT)
	rm -f FOO

