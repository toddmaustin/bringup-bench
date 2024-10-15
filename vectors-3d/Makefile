
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=vectors-3d.o

PROG=vectors-3d

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO vectors-3d.out
	rm -f FOO

