#include "libmin.h"

// Define your custom CSR number
#define CSR_MPRIVREGCFG 0x0a0

// Inline helpers
static inline uint64_t
read_mprivregcfg(void)
{
  uint64_t value;
  asm volatile ("csrr %0, %1" : "=r"(value) : "i"(CSR_MPRIVREGCFG));
  return value;
}

static inline void
write_mprivregcfg(uint64_t value)
{
  asm volatile ("csrw %0, %1" :: "i"(CSR_MPRIVREGCFG), "rK"(value));
}

// Predefined memory values
volatile uint64_t x = 35;
volatile uint64_t max = 25;

int
 main(void)
{
  //
  // mprivregcfg tests
  //
  libmin_printf("** Running CSR[privreg] tests...\n");

  uint64_t val;

  // read reset value
  val = read_mprivregcfg();
  libmin_printf("Initial mprivregcfg = 0x%lx\n", val);

  // enable private register semantics (bit 0 = 1)
  write_mprivregcfg(1);

  val = read_mprivregcfg();
  libmin_printf("After enable, mprivregcfg = 0x%lx\n", val);


  // do some secret computation
  libmin_printf("Initial inputs: x:%lu, max:%lu\n", x, max);

  // inline assembly block
  asm volatile (
    // load third-party encrypted operands
    "ld        /*p0*/t3, (%0)\n\t"   // p0 = x
    "ld        /*p1*/t4, (%1)\n\t"   // p1 = max

    // Condition: (max < x)?
    // "slt       /*p2*/t5, x1, x2\n\t" // Mojo-V test: no secret inputs
    // "jalr         ra, 64(t4)\n\t"
    // "sw        t0, (t3)\n\t"
    // "bne       t3, t0, .+12\n\t"
    // "bne       t0, t3, .+12\n\t"
    // "slt       t0, /*p1*/t4, /*p0*/t3\n\t" // Mojo-V test: should have secret dest
    "slt       /*p2*/t5, /*p1*/t4, /*p0*/t3\n\t" // p2 = (p1 < p0) ? 1 : 0

    // Build data-oblivious conditional result
    "czero.eqz /*p0*/t3, /*p0*/t3, /*p2*/t5\n\t" // if p2==0 => p0=0, else p0=x
    "czero.nez /*p1*/t4, /*p1*/t4, /*p2*/t5\n\t" // if p2!=0 => p1=0, else p1=max
    "or        /*p3*/t6, /*p0*/t3, /*p1*/t4\n\t" // select: p3 = (x if x>max else max)

    // Store third-party encrypted (potentially) new max value
    "sd     /*p3*/t6, (%1)\n\t"

    :
    : "r" (&x), "r" (&max)   // input operands
    : "t3", "t4", "t5", "t6" // clobbered registers
  );

  libmin_printf("Final results:   x:%lu, max:%lu\n", x, max);


  // disable private register semantics (write 0)
  write_mprivregcfg(0);

  val = read_mprivregcfg();
  libmin_printf("After disable, mprivregcfg = 0x%lx\n", val);

  libmin_success();
}

