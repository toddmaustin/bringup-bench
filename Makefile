define HELP_TEXT
Please choose one of the following target
  config-vip     - configure VIP-Bench to target the VIP-Bench functional library
  config-meso    - configure VIP-Bench to target Agita Labs' Mesosphere secure computation SDK
  config-seal    - configure VIP-Bench to target Microsoft's SEAL Homomorphic Encryption library
  run-tests      - clean, build, and test all benchmarks in all target modes (NA,DO,ENC)
  all-clean      - clean all benchmark directories

Within individual directories, the following Makefile targets are also available:
  clean          - delete all generated files
  build          - build the binary
  test           - run the standard test on the binary

Note that benchmark builds must be parameterized with the build MODE, such as:
  TARGET=host       - build in NATIVE mode, non-data-oblivious build without encryption
  TARGET=standalone - build in DATA-OBLIVIOUS ENCRYPTED mode, data-oblivious build with encrpytion

Example benchmark builds:
  make MODE=na clean build test
  make MODE=enc build
  make MODE=do clean
endef

export HELP_TEXT

error:
	@echo "$$HELP_TEXT"

#
# END of user-modifiable variables
#
BMARKS = ackermann anagram banner boyer-moore-search bubble-sort c-interp cipher dhrystone distinctness fft-int flood-fill frac-calc hanoi heapsort kepler longdiv mandelbrot mersenne natlog nr-solver parrondo pascal shortest-path sieve skeleton totient

OPT_CFLAGS = -O0 -g

ifeq ($(TARGET), host)
TARGET_CFLAGS = -DLIBTAR_HOST
TARGET_LIBS =
TARGET_SIM =
TARGET_DIFF = diff
TARGET_EXE = $(PROG).host
else ifeq ($(TARGET), standalone)
TARGET_CFLAGS = -DLIBTARG_SA
TARGET_LIBS =
TARGET_SIM =
TARGET_DIFF = diff
TARGET_EXE = $(PROG).sa
else
# default is an error
#$(error No build TARGET defined, e.g., make TARGET=host ...)
endif

CFLAGS = -Wall $(OPT_CFLAGS) -Wno-strict-aliasing $(TARGET_CFLAGS) $(LOCAL_CFLAGS)
OBJS = $(LOCAL_OBJS) ../common/libmin.o ../common/libtarg.o
LIBS = $(LOCAL_LIBS) $(TARGET_LIBS)

build: $(TARGET_EXE)

%.o: %.c
ifeq ($(TARGET), host)
	$(CC) $(CFLAGS) -DTARGET_HOST -I../common/ -o $(notdir $@) -c $<
else ifeq ($(TARGET), standalone)
	$(CC) $(CFLAGS) -DTARGET_SA -I../common/ -o $(notdir $@) -c $<
else
	$(error MODE is not defined (add: TARGET={host|sa}).)
endif

$(TARGET_EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(notdir $^) $(LIBS)

clean:
	rm -f $(PROG).host $(PROG).sa *.o core mem.out *.log FOO $(LOCAL_CLEAN)


#
# top-level Makefile interfaces
#

config-vip:
	@echo "Configuring VIP-Bench for VIP functional library..."
	ln -sf configs/config.mk.vip config.mk
	ln -sf configs/config.h.vip config.h

config-mesa:
	@echo "Configuring VIP-Bench for Agita Labs' Mesosphere SDK..."
	ln -sf configs/config.mk.meso config.mk
	ln -sf configs/config.h.meso config.h

config-seal:
	@echo "Configuring VIP-Bench for Microsoft SEAL HE library..."
	ln -sf configs/config.mk.seal config.mk
	ln -sf configs/config.h.seal config.h

run-tests:
	@for _BMARK in $(BMARKS) ; do \
	  for _TARGET in host standalone ; do \
	    cd $$_BMARK ; \
	    echo "--------------------------------" ; \
	    echo "Running "$$_BMARK" in TARGET="$$_TARGET ; \
	    echo "--------------------------------" ; \
	    $(MAKE) TARGET=$$_TARGET clean build test || exit 1; \
	    cd .. ; \
	  done \
	done

all-clean: clean
	@for _BMARK in $(BMARKS) ; do \
	  for _TARGET in host standalone ; do \
	    cd $$_BMARK ; \
	    echo "--------------------------------" ; \
	    echo "Running "$$_BMARK" in TARGET="$$_TARGET ; \
	    echo "--------------------------------" ; \
	    $(MAKE) TARGET=$$_TARGET clean ; \
	    cd .. ; \
	  done \
	done

