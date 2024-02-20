
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=max-subseq.o

PROG=max-subseq

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO max-subseq.out
	rm -f FOO

