
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=ransac.o

PROG=ransac

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO ransac.$(TARGET_REFEXT)
	rm -f FOO

