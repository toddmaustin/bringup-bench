LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=chi-squared.o

PROG=chi-squared

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO
	$(TARGET_DIFF) FOO chi-squared.$(TARGET_REFEXT)
	rm -f FOO
