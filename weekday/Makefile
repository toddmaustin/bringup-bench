
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=weekday.o

PROG=weekday

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO weekday.$(TARGET_REFEXT)
	rm -f FOO

