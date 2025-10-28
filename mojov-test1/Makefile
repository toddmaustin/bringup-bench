
LOCAL_CFLAGS=-std=c23
LOCAL_LIBS=
LOCAL_OBJS=mojov-test1.o ../common/simon.o

PROG=mojov-test1

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO mojov-test1.$(TARGET_REFEXT)
	rm -f FOO

