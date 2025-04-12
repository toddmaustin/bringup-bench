
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=packet-filter.o

PROG=packet-filter

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO packet-filter.$(TARGET_REFEXT)
	rm -f FOO

