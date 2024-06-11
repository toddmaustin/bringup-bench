
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=regex-parser.o

PROG=regex-parser

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO regex-parser.out
	rm -f FOO

