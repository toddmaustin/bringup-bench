
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=avl-tree.o element.o avlcore.o

PROG=avl-tree

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO avl-tree.out
	rm -f FOO

