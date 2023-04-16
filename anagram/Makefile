
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=anagram.o

PROG=anagram

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) words > FOO 
	$(TARGET_DIFF) FOO anagram.out
	rm -f FOO

