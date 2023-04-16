
LOCAL_CFLAGS= -I../common
LOCAL_LIBS=
LOCAL_CLEAN=FOO.bmp

PROG=image-filters
OBJS=image-filters.o helpers.o ../common/utils.o ../common/mathlib.o

include ../config.mk
include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) -s original.bmp FOO.bmp
	$(TARGET_DIFF) FOO.bmp image-filters.out
	rm -f FOO.bmp
	-$(TARGET_SIM) ./$(TARGET_EXE) -e original.bmp FOO.bmp
	$(TARGET_DIFF) FOO.bmp image-filters1.out
	rm -f FOO.bmp
