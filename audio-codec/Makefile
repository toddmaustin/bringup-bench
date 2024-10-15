
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=audio-codec.o

PROG=audio-codec

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO audio-codec.out
	rm -f FOO

