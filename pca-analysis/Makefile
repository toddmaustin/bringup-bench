LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=pca-analysis.o

PROG=pca-analysis

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO
	$(TARGET_DIFF) FOO pca-analysis.$(TARGET_REFEXT)
	rm -f FOO
