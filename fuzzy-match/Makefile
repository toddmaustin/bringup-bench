
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=fuzzy-match.o

PROG=fuzzy-match

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO fuzzy-match.$(TARGET_REFEXT)
	rm -f FOO

