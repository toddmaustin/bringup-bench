
LOCAL_CFLAGS=
LOCAL_LIBS=
LOCAL_OBJS=satomi.o cnf_reader.o solver.o

PROG=satomi

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) zebra_v155_c1135.cnf > FOO 
	$(TARGET_DIFF) FOO satomi.out
	rm -f FOO

