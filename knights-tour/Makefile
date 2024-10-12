
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=knights-tour.o

PROG=knights-tour

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO knights-tour.out
	rm -f FOO

