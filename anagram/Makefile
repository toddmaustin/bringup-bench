
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=anagram.o

PROG=anagram

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO anagram.$(TARGET_REFEXT)
	rm -f FOO

