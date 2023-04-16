
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=banner.o

PROG=banner

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO banner.out
	rm -f FOO

