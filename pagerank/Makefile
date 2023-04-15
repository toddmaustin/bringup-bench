
LOCAL_CFLAGS= -I../common
LOCAL_LIBS=

PROG=pagerank
OBJS=graph.o main.o pagerank.o pageutil.o ../common/utils.o

include ../config.mk
include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) --fmt DIMACS --style pull --df 0.85 --filename r1000a.dimacs > FOO 
	$(TARGET_DIFF) FOO pagerank.out
	rm -f FOO

