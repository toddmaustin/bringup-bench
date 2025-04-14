
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=sudoku-solver.o

PROG=sudoku-solver

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO sudoku-solver.$(TARGET_REFEXT)
	rm -f FOO

