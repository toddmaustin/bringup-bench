
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=life.o

PROG=life

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO life.out
	rm -f FOO

