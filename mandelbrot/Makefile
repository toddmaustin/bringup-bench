
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=mandelbrot.o

PROG=mandelbrot

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO mandelbrot.out
	rm -f FOO

