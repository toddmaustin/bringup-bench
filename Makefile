define HELP_TEXT
Please choose one of the following targets:
  run-tests      - clean, build, and test all benchmarks for the specified TARGET mode (host,standalone,simple,spike[62/32],spike[64/32]-pk)
  all-clean      - clean all benchmark directories for all TARGET modes
  spike-build    - build RISC-V Spike simulator extensions for bringup-bench

Within individual directories, the following Makefile targets are also available:
  clean          - delete all generated files
  build          - build the binary
  test           - run the standard test on the binary

Note that benchmark builds must be parameterized with the build MODE, such as:
  TARGET=host       - build benchmarks to run on a Linux host
  TARGET=standalone - build benchmarks to run in standalone mode (a virtual bare-metal CPU)
  TARGET=simple     - build benchmarks to run on the RISC-V Simple_System simulation testing environment
  TARGET=spike[64/32]    - build benchmarks to run on the RISC-V 32-bit Spike Instruction Set Simulator (ISS) with Simple_System devices
  TARGET=spike[64/32]-pk - build benchmarks to run on the RISC-V 64-bit Spike Instruction Set Simulator (ISS) with proxy kernel (pk)

Example benchmark builds:
  make TARGET=host clean build test
  make TARGET=standalone build
  make TARGET=simple clean
  make all-clean
  make TARGET=simple run-tests
endef

export HELP_TEXT

error:
	@echo "$$HELP_TEXT"

#
# END of user-modifiable variables
#
BMARKS = ackermann aes anagram audio-codec avl-tree banner bit-kernels blake2b bloom-filter boyer-moore-search bubble-sort c-interp ccmac checkers cipher congrad connect4-minimax convex-hull dhrystone distinctness donut fft-int flood-fill frac-calc fuzzy-match fy-shuffle gcd-list grad-descent graph-tests hanoi heapsort heat-calc huff-encode idct-alg indirect-test k-means kadane kepler knapsack knights-tour life longdiv lu-decomp lz-compress mandelbrot matmult max-subseq mersenne minspan monte-carlo murmur-hash n-queens natlog nbody-sim nr-solver packet-filter parrondo pascal pi-calc primal-test priority-queue quaternions qsort-demo qsort-test quine rabinkarp-search rand-test ransac regex-parser rho-factor rle-compress rsa-cipher sat-solver shortest-path sieve simple-grep skeleton spelt2num spirograph strange sudoku-solver tetris-sim tiny-NN topo-sort totient transcend uniquify vectors-3d verlat weekday

#OPT_CFLAGS = -O0 -g
OPT_CFLAGS = -O3 -g

ifeq ($(TARGET), host)
TARGET_CC = gcc
#TARGET_CC = clang
TARGET_AR = ar
TARGET_CFLAGS = -DTARGET_HOST -DTARGET_PERFHOOKS
TARGET_LIBS =
TARGET_SIM =
TARGET_DIFF = diff
TARGET_EXE = $(PROG).host
TARGET_CLEAN =
TARGET_CONFIGURED = 1
TARGET_REFEXT = out
else ifeq ($(TARGET), standalone)
TARGET_CC = gcc
#TARGET_CC = clang
TARGET_AR = ar
TARGET_CFLAGS = -DTARGET_SA
TARGET_LIBS =
TARGET_SIM =
TARGET_DIFF = diff
TARGET_EXE = $(PROG).sa
TARGET_CLEAN =
TARGET_CONFIGURED = 1
TARGET_REFEXT = out
else ifeq ($(TARGET), hashalone-host)
TARGET_CC = gcc
#TARGET_CC = clang
TARGET_AR = ar
TARGET_CFLAGS = -DTARGET_HAHOST
TARGET_LIBS =
TARGET_SIM =
TARGET_DIFF = diff
TARGET_EXE = $(PROG).hahost
TARGET_CLEAN =
TARGET_EXCLUDES = 
TARGET_CONFIGURED = 1
TARGET_REFEXT = hash
else ifeq ($(TARGET), hashalone-spike32)
TARGET_CC = riscv64-unknown-elf-gcc
#TARGET_CC = riscv64-unknown-elf-clang
TARGET_AR = riscv64-unknown-elf-ar
TARGET_CFLAGS = -DTARGET_HASPIKE -DLIBMIN_MALLOC_ALIGN_BYTES=8 -march=rv32imczicsr -mabi=ilp32 -static -mcmodel=medlow -Wall -g -Os -fvisibility=hidden -nostdlib -nostartfiles -ffreestanding # -MMD -mcmodel=medany 
TARGET_LIBS = -lgcc
TARGET_SIM = ../../riscv-isa-sim/build/spike --isa=RV32IMC --extlib=../target/spike_mmio_plugin.so -m0x100000:0x820000 --device=spike_mmio_plugin,0x20000
TARGET_EXE = $(PROG).haspike
TARGET_CONFIGURED = 1
TARGET_DIFF = diff
TARGET_CLEAN = *.d ibex_simple_system_pcount.csv
TARGET_EXCLUDES = anagram c-interp checkers lz-compress rho-factor rsa-cipher spelt2num
TARGET_REFEXT = hash
else ifeq ($(TARGET), hashalone-spike64)
TARGET_CC = riscv64-unknown-elf-gcc
#TARGET_CC = riscv64-unknown-elf-clang
TARGET_AR = riscv64-unknown-elf-ar
TARGET_CFLAGS = -DTARGET_HASPIKE -DLIBMIN_MALLOC_ALIGN_BYTES=8 -march=rv64gc -mabi=lp64d -static -mcmodel=medlow -Wall -g -Os -fvisibility=hidden -nostdlib -nostartfiles -ffreestanding # -MMD -mcmodel=medany 
TARGET_LIBS = -lgcc
TARGET_SIM = ../../riscv-isa-sim/build/spike --isa=rv64gc --extlib=../target/spike_mmio_plugin.so -m0x100000:0x820000 --device=spike_mmio_plugin,0x20000
TARGET_EXE = $(PROG).haspike
TARGET_CONFIGURED = 1
TARGET_DIFF = diff
TARGET_CLEAN = *.d ibex_simple_system_pcount.csv
TARGET_EXCLUDES = anagram c-interp checkers lz-compress rsa-cipher spelt2num
TARGET_REFEXT = hash
else ifeq ($(TARGET), simple)
TARGET_CC = riscv64-unknown-elf-gcc
#TARGET_CC = riscv64-unknown-elf-clang
TARGET_AR = riscv64-unknown-elf-ar
TARGET_CFLAGS = -DTARGET_SIMPLE -march=rv32imczicsr -mabi=ilp32 -static -mcmodel=medlow -Wall -g -Os -fvisibility=hidden -nostdlib -nostartfiles -ffreestanding # -MMD -mcmodel=medany
TARGET_LIBS = -lgcc
TARGET_SIM = ../target/simple_sim.sh ../../../Snowflake-IoT/ibex/build/lowrisc_ibex_ibex_simple_system_0/sim-verilator/Vibex_simple_system
TARGET_DIFF = mv ibex_simple_system.log FOO; diff
TARGET_EXE = $(PROG).elf
TARGET_CLEAN = *.d ibex_simple_system_pcount.csv
TARGET_EXCLUDES = ackermann anagram c-interp checkers donut lz-compress pi-calc rho-factor rsa-cipher spelt2num
TARGET_CONFIGURED = 1
TARGET_REFEXT = out
else ifeq ($(TARGET), spike32)
TARGET_CC = riscv64-unknown-elf-gcc
#TARGET_CC = riscv64-unknown-elf-clang
TARGET_AR = riscv64-unknown-elf-ar
TARGET_CFLAGS = -DTARGET_SPIKE -DLIBMIN_MALLOC_ALIGN_BYTES=8 -march=rv32imczicsr -mabi=ilp32 -static -mcmodel=medlow -Wall -g -Os -fvisibility=hidden -nostdlib -nostartfiles -ffreestanding # -MMD -mcmodel=medany 
TARGET_LIBS = -lgcc
TARGET_SIM = ../../riscv-isa-sim/build/spike --isa=RV32IMC --extlib=../target/spike_mmio_plugin.so -m0x100000:0x820000 --device=spike_mmio_plugin,0x20000
TARGET_DIFF = diff
TARGET_EXE = $(PROG).elf
TARGET_CLEAN = *.d ibex_simple_system_pcount.csv
TARGET_EXCLUDES = anagram c-interp checkers lz-compress rho-factor rsa-cipher spelt2num
TARGET_CONFIGURED = 1
TARGET_REFEXT = out
else ifeq ($(TARGET), spike64)
TARGET_CC = riscv64-unknown-elf-gcc
#TARGET_CC = riscv64-unknown-elf-clang
TARGET_AR = riscv64-unknown-elf-ar
TARGET_CFLAGS = -DTARGET_SPIKE -DLIBMIN_MALLOC_ALIGN_BYTES=8 -march=rv64gc -mabi=lp64d -static -mcmodel=medlow -Wall -g -Os -fvisibility=hidden -nostdlib -nostartfiles -ffreestanding # -MMD -mcmodel=medany 
TARGET_LIBS = -lgcc
TARGET_SIM = ../../riscv-isa-sim/build/spike --isa=rv64gc --extlib=../target/spike_mmio_plugin.so -m0x100000:0x820000 --device=spike_mmio_plugin,0x20000
TARGET_DIFF = diff
TARGET_EXE = $(PROG).elf
TARGET_CLEAN = *.d ibex_simple_system_pcount.csv
TARGET_EXCLUDES = anagram c-interp checkers lz-compress rsa-cipher spelt2num
TARGET_CONFIGURED = 1
TARGET_REFEXT = out
else ifeq ($(TARGET), spike32-pk)
TARGET_CC = riscv64-unknown-elf-gcc
#TARGET_CC = riscv64-unknown-elf-clang
TARGET_AR = riscv64-unknown-elf-ar
TARGET_CFLAGS = -DTARGET_SPIKE_PK -DLIBMIN_MALLOC_ALIGN_BYTES=8 -march=rv32imczicsr -mabi=ilp32 -static -mcmodel=medlow -Wall -g -Os -fvisibility=hidden -ffreestanding # -MMD -mcmodel=medany 
TARGET_LIBS = -lgcc
TARGET_SIM = ../../riscv-isa-sim/build/spike --isa=RV32IMC pk32
TARGET_DIFF = diff
TARGET_EXE = $(PROG).elf
TARGET_CLEAN = 
TARGET_EXCLUDES = anagram c-interp checkers lz-compress rho-factor rsa-cipher spelt2num
TARGET_CONFIGURED = 1
TARGET_REFEXT = out
else ifeq ($(TARGET), spike64-pk)
TARGET_CC = riscv64-unknown-elf-gcc
#TARGET_CC = riscv64-unknown-elf-clang
TARGET_AR = riscv64-unknown-elf-ar
TARGET_CFLAGS = -DTARGET_SPIKE_PK -DLIBMIN_MALLOC_ALIGN_BYTES=8 -march=rv64gc -mabi=lp64d -static -mcmodel=medlow -Wall -g -Os -fvisibility=hidden -ffreestanding # -MMD -mcmodel=medany 
TARGET_LIBS = -lgcc
TARGET_SIM = ../../riscv-isa-sim/build/spike --isa=rv64gc pk
TARGET_DIFF = diff
TARGET_EXE = $(PROG).elf
TARGET_CLEAN = 
TARGET_EXCLUDES =
TARGET_CONFIGURED = 1
TARGET_REFEXT = out
else
# default is an unconfigured
TARGET_CONFIGURED = 0
endif

TARGET_BMARKS = $(filter-out $(TARGET_EXCLUDES), $(BMARKS))

CFLAGS = -Wall $(OPT_CFLAGS) -Wno-strict-aliasing $(TARGET_CFLAGS) $(LOCAL_CFLAGS)
OBJS = $(LOCAL_OBJS) ../target/libtarg.o
__LIBMIN_SRCS = libmin_abs.c libmin_acos.c libmin_asin.c libmin_atan.c libmin_atof.c \
  libmin_atoi.c libmin_atol.c libmin_ctype.c libmin_exp.c \
  libmin_fabs.c libmin_fail.c libmin_floor.c libmin_fnv1a.c libmin_getopt.c libmin_malloc.c libmin_mclose.c \
  libmin_memcmp.c libmin_memcpy.c libmin_memmove.c libmin_memset.c libmin_meof.c libmin_mgetc.c \
  libmin_mgets.c libmin_mopen.c libmin_mread.c libmin_msize.c libmin_pow.c libmin_printf.c \
  libmin_putc.c libmin_puts.c libmin_qsort.c libmin_rand.c libmin_rempio2.c libmin_scalbn.c \
  libmin_scanf.c libmin_sincos.c libmin_sqrt.c libmin_strcat.c libmin_strchr.c libmin_strcmp.c \
  libmin_strcpy.c libmin_strcspn.c libmin_strdup.c libmin_strlen.c libmin_strncat.c libmin_strncmp.c \
  libmin_strncpy.c libmin_strpbrk.c libmin_strrchr.c libmin_strspn.c libmin_strstr.c libmin_strcasestr.c \
  libmin_strtok.c libmin_strtol.c libmin_success.c libmin_strncasecmp.c
LIBMIN_SRCS = $(addprefix ../common/,$(basename $(__LIBMIN_SRCS)))
LIBMIN_OBJS = $(addprefix ../common/,$(addsuffix .o,$(basename $(__LIBMIN_SRCS))))

LIBS = ../common/libmin.a

build: $(TARGET_EXE)

%.o: %.c
	$(TARGET_CC) $(CFLAGS) -I../common/ -I../target/ -o $@ -c $<

../common/libmin.a: $(LIBMIN_OBJS)
	$(TARGET_AR) rcs ../common/libmin.a $(LIBMIN_OBJS)

$(TARGET_EXE): $(OBJS) $(LIBS)
ifeq ($(TARGET), host)
	$(TARGET_CC) $(CFLAGS) -o $@ $^ $(LIBS) $(TARGET_LIBS)
else ifeq ($(TARGET), standalone)
	$(TARGET_CC) $(CFLAGS) -o $@ $^ $(LIBS) $(TARGET_LIBS)
else ifeq ($(TARGET), hashalone-host)
	$(TARGET_CC) $(CFLAGS) -o $@ $^ $(LIBS) $(TARGET_LIBS)
else ifeq ($(TARGET), hashalone-spike32)
	$(TARGET_CC) $(CFLAGS) -T ../target/spike-map.ld $^ ../target/spike-crt0.S -o $@ $(LIBS) $(TARGET_LIBS)
else ifeq ($(TARGET), hashalone-spike64)
	$(TARGET_CC) $(CFLAGS) -T ../target/spike-map.ld $^ ../target/spike-crt0.S -o $@ $(LIBS) $(TARGET_LIBS)
else ifeq ($(TARGET), simple)
	$(TARGET_CC) $(CFLAGS) -T ../target/simple-map.ld $^ ../target/simple-crt0.S -o $@ $(LIBS) $(TARGET_LIBS)
else ifeq ($(TARGET), spike32)
	$(TARGET_CC) $(CFLAGS) -T ../target/spike-map.ld $^ ../target/spike-crt0.S -o $@ $(LIBS) $(TARGET_LIBS)
else ifeq ($(TARGET), spike64)
	$(TARGET_CC) $(CFLAGS) -T ../target/spike-map.ld $^ ../target/spike-crt0.S -o $@ $(LIBS) $(TARGET_LIBS)
else ifeq ($(TARGET), spike32-pk)
	$(TARGET_CC) $(CFLAGS) -o $@ $^ $(LIBS) $(TARGET_LIBS)
else ifeq ($(TARGET), spike64-pk)
	$(TARGET_CC) $(CFLAGS) -o $@ $^ $(LIBS) $(TARGET_LIBS)
else
	$(error MODE is not defined (add: TARGET={host|sa}).)
endif

clean:
	rm -f $(PROG).host $(PROG).sa $(PROG).elf $(PROG).hahost $(PROG).haspike *.o ../common/*.o ../target/*.o ../common/libmin.a *.d ../common/*.d core mem.out *.log FOO $(LOCAL_CLEAN) $(TARGET_CLEAN)


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

clean-all all-clean:
	@for _BMARK in $(BMARKS) ; do \
	  for _TARGET in host standalone hashalone-host simple spike hashalone-host hashalone-spike ; do \
	    cd $$_BMARK ; \
	    echo "--------------------------------" ; \
	    echo "Cleaning "$$_BMARK" in TARGET="$$_TARGET ; \
	    echo "--------------------------------" ; \
	    $(MAKE) TARGET=$$_TARGET clean ; \
	    cd .. ; \
	  done \
	done

spike-build:
	$(MAKE) -C target clean build

