
LOCAL_CFLAGS= -I../common
LOCAL_LIBS=

PROG=bubble-sort
OBJS=bubble-sort.o ../common/utils.o

include ../config.mk
include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO bubble-sort.out
	rm -f FOO

