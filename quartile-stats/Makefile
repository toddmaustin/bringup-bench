LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=quartile-stats.o

PROG=quartile-stats

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO
	$(TARGET_DIFF) FOO quartile-stats.$(TARGET_REFEXT)
	rm -f FOO
