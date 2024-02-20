define HELP_TEXT
Please choose one of the following targets:
  run-tests      - clean, build, and test all benchmarks in all target modes (NA,DO,ENC)
  all-clean      - clean all benchmark directories

Within individual directories, the following Makefile targets are also available:
  clean          - delete all generated files
  build          - build the binary
  test           - run the standard test on the binary

Note that benchmark builds must be parameterized with the build MODE, such as:
  TARGET=host       - build benchmarks to run on a Linux host
  TARGET=standalone - build benchmarks to run in standalone mode (a virtual bare-metal CPU)
  TARGET=simple     - build benchmarks to run on the RISC-V Simple_System simulation testing environment

Example benchmark builds:
  make TARGET=host clean build test
  make TARGET=standalone build
  make TARGET=simple clean
endef

export HELP_TEXT

error:
	@echo "$$HELP_TEXT"

#
# END of user-modifiable variables
#
BMARKS = ackermann anagram banner boyer-moore-search bubble-sort c-interp checkers cipher dhrystone distinctness donut fft-int flood-fill frac-calc fy-shuffle gcd-list hanoi heapsort kepler life longdiv lz-compress mandelbrot max-subseq mersenne minspan natlog nr-solver parrondo pascal pi-calc primal-test quine rabinkarp-search rho-factor shortest-path sieve skeleton spelt2num strange topo-sort totient weekday

OPT_CFLAGS = -O6 -g

ifeq ($(TARGET), host)
TARGET_CC = gcc
TARGET_CFLAGS = -DTARGET_HOST
TARGET_LIBS =
TARGET_SIM =
TARGET_DIFF = diff
TARGET_EXE = $(PROG).host
TARGET_CLEAN =
TARGET_BMARKS = $(BMARKS)
TARGET_CONFIGURED = 1
else ifeq ($(TARGET), standalone)
TARGET_CC = gcc
TARGET_CFLAGS = -DTARGET_SA
TARGET_LIBS =
TARGET_SIM =
TARGET_DIFF = diff
TARGET_EXE = $(PROG).sa
TARGET_CLEAN =
TARGET_BMARKS = $(BMARKS)
TARGET_CONFIGURED = 1
else ifeq ($(TARGET), simple)
TARGET_CC = riscv32-unknown-elf-gcc
TARGET_CFLAGS = -DTARGET_SIMPLE -march=rv32imc -mabi=ilp32 -static -mcmodel=medany -Wall -g -Os -fvisibility=hidden -nostdlib -nostartfiles -ffreestanding  -MMD
TARGET_LIBS = -lgcc
TARGET_SIM = ../target/simple_sim.sh ../../Snowflake-IoT/ibex/build/lowrisc_ibex_ibex_simple_system_0/sim-verilator/Vibex_simple_system
TARGET_DIFF = mv ibex_simple_system.log FOO; diff
TARGET_EXE = $(PROG).elf
TARGET_CLEAN = *.d ibex_simple_system_pcount.csv
TARGET_BMARKS = banner boyer-moore-search bubble-sort cipher dhrystone distinctness fft-int flood-fill frac-calc fy-shuffle gcd-list hanoi heapsort kepler life longdiv mandelbrot max-subseq mersenne minspan natlog nr-solver parrondo pascal primal-test rabinkarp-search shortest-path sieve skeleton strange topo-sort totient weekday
TARGET_CONFIGURED = 1
else
# default is an unconfigured
TARGET_CONFIGURED = 0
endif

CFLAGS = -Wall $(OPT_CFLAGS) -Wno-strict-aliasing $(TARGET_CFLAGS) $(LOCAL_CFLAGS)
OBJS = $(LOCAL_OBJS) ../common/libmin.o ../common/libtarg.o
LIBS = $(LOCAL_LIBS) $(TARGET_LIBS)

build: $(TARGET_EXE)

%.o: %.c
	$(TARGET_CC) $(CFLAGS) -I../common/ -o $(notdir $@) -c $<

$(TARGET_EXE): $(OBJS)
ifeq ($(TARGET), host)
	$(TARGET_CC) $(CFLAGS) -o $@ $(notdir $^) $(LIBS)
else ifeq ($(TARGET), standalone)
	$(TARGET_CC) $(CFLAGS) -o $@ $(notdir $^) $(LIBS)
else ifeq ($(TARGET), simple)
	$(TARGET_CC) $(CFLAGS) -T ../target/simple-map.ld $(notdir $^) ../target/simple-crt0.S -o $@ $(LIBS)
else
	$(error MODE is not defined (add: TARGET={host|sa}).)
endif

clean:
	rm -f $(PROG).host $(PROG).sa $(PROG).elf *.o core mem.out *.log FOO $(LOCAL_CLEAN) $(TARGET_CLEAN)


#
# top-level Makefile interfaces
#

run-tests:
ifeq ($(TARGET_CONFIGURED), 0)
	@echo "'run-tests' command requires a TARGET definition." ; \
	echo "" ; \
	echo "$$HELP_TEXT"
else
	@for _BMARK in $(TARGET_BMARKS) ; do \
	  cd $$_BMARK ; \
	  echo "--------------------------------" ; \
	  echo "Running "$$_BMARK" in TARGET="$$TARGET ; \
	  echo "--------------------------------" ; \
	  $(MAKE) TARGET=$$TARGET clean build test || exit 1; \
	  cd .. ; \
	done
endif 

clean-all all-clean: clean
	@for _BMARK in $(BMARKS) ; do \
	  for _TARGET in host standalone simple ; do \
	    cd $$_BMARK ; \
	    echo "--------------------------------" ; \
	    echo "Cleaning "$$_BMARK" in TARGET="$$_TARGET ; \
	    echo "--------------------------------" ; \
	    $(MAKE) TARGET=$$_TARGET clean ; \
	    cd .. ; \
	  done \
	done


