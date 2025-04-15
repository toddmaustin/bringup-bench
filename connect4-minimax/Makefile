
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=connect4-minimax.o

PROG=connect4-minimax

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO connect4-minimax.$(TARGET_REFEXT)
	rm -f FOO

