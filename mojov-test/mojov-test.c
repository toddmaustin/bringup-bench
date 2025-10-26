#include "libmin.h"
#include "simon.h"

typedef unsigned __int128 uint128_t;

// Mojo-V asm instruction definitions (using the format-friendly .insn directive in GNU AS
#define LDE(rd,base,ofs) ".insn i 0xb, 0x0, " #rd ", " #base ", " #ofs "\n\t"
#define SDE(src,base,ofs) ".insn s 0xb, 0x1, " #src ", " #ofs "(" #base ")\n\t"

// Define your custom CSR number
#define CSR_MPRIVREGCFG 0x0a0

static void
print_mprivregcfg(uint64_t val)
{
  libmin_printf("(mojov_en:%s, key_valid:%s, format_sel:%s, mojov_ver:%u)",
                (val & 0x01) ? "t" : "f",
                (val & 0x02) ? "t" : "f",
                (val & 0x04) ? "strong" : "weak",
                (val >> 3) & 0xff);
}

// Inline helpers
static inline uint64_t
read_mprivregcfg(void)
{
  uint64_t value;
  __asm__ volatile ("csrr %0, %1" : "=r"(value) : "i"(CSR_MPRIVREGCFG));
  return value;
}

static inline void
write_mprivregcfg(uint64_t value)
{
  __asm__ volatile ("csrw %0, %1" :: "i"(CSR_MPRIVREGCFG), "rK"(value));
}

// Predefined memory values
uint64_t x = 35;
uint64_t max = 25;

uint128_t x_enc;
uint128_t max_enc;
uint128_t bogus_enc = 42;

int
main(void)
{

  // initilize cipher engine, for checking results
  #define MOJOV_PT_SIG   0xdeadbeef
  union mojov_memfmt_t {
    uint128_t ct;     // ciphertext

    struct {          // plaintext
      uint64_t val;     // register plaintext value
      uint32_t salt;    // random salt
      uint32_t sig;     // fixed signature
    } pt;
  };
  uint128_t simon_key = GEN128(0x0f0e0d0c0b0a0908, 0x0706050403020100);
  simon_state_t simon_state;
  simon_128_128_keyexpand(&simon_state, simon_key, 68);

  //
  // mprivregcfg tests
  //
  libmin_printf("** Running CSR[privreg] tests...\n");

  uint64_t val;

  // read reset value
  val = read_mprivregcfg();
  libmin_printf("Initial mprivregcfg = 0x%lx, ", val);
  print_mprivregcfg(val);
  libmin_printf("\n");

  // enable private register semantics (bit 0 = 1)
  write_mprivregcfg(1);

  val = read_mprivregcfg();
  libmin_printf("After enable, mprivregcfg = 0x%lx, ", val);
  print_mprivregcfg(val);
  libmin_printf("\n");


  // do some secret computation
  libmin_printf("Initial inputs: x:%lu, max:%lu\n", x, max);

  // inline assembly block
  __asm__ volatile (
    // first encrypt the public X and MAX values
    "ld t3, (%0)\n\t"
    SDE(t3,%2,0)
    "ld t3, (%1)\n\t"
    SDE(t3,%3,0)

    // test-load a bogus ciphertext value -- it should get an exception
    // LDE(t3, %4, 0)

    // cannot ld/sd a secret register
    // "sd t3, (%0)\n\t"
    // "sd t0, (%0)\n\t"

    // load third-party encrypted operands
    LDE(t3, %2, 0)
    LDE(t4, %3, 0)

    // Condition: (max < x)?
    // "slt       /*p2*/t5, x1, x2\n\t" // Mojo-V test: no secret inputs
    // "jalr         ra, 64(t4)\n\t"
    // "sw        t5, (t3)\n\t"
    // "bne       t3, t0, .+12\n\t"
    // "bne       t0, t3, .+12\n\t"
    // "slt       t0, /*p1*/t4, /*p0*/t3\n\t" // Mojo-V test: should have secret dest
    "slt       /*p2*/t5, /*p1*/t4, /*p0*/t3\n\t" /* p2 = (p1 < p0) ? 1 : 0 */

    // try to move the secret predicate, via integer to FP register/ moves/converts
    "fmv.w.x      f1, t2\n\t"
    "fcvt.s.w     f3, t2\n\t"
    // "fmv.w.x      f1, t5\n\t"
    // "fmv.d.x      f2, t5\n\t"
    // "fcvt.s.w     f3, t5\n\t"
    // "fcvt.s.wu    f3, t5\n\t"
    // "fcvt.s.l     f5, t5\n\t"
    // "fcvt.s.lu    f6, t5\n\t"
    // "fcvt.d.w     f1, t5\n\t"
    // "fcvt.d.wu    f2, t5\n\t"
    // "fcvt.d.l     f3, t5\n\t"
    // "fcvt.d.lu    f4, t5\n\t"

    // Build data-oblivious conditional result
    "czero.eqz /*p0*/t3, /*p0*/t3, /*p2*/t5\n\t" // if p2==0 => p0=0, else p0=x
    "czero.nez /*p1*/t4, /*p1*/t4, /*p2*/t5\n\t" // if p2!=0 => p1=0, else p1=max
    "or        /*p3*/t6, /*p0*/t3, /*p1*/t4\n\t" // select: p3 = (x if x>max else max)

    // Store third-party encrypted (potentially) new max value
    SDE(t6,%3,0)

    :
    : "r" (&x), "r" (&max), "r" (&x_enc), "r" (&max_enc), "r" (&bogus_enc) // input operands
    : "t3", "t4", "t5", "t6" // clobbered registers
  );

  // decrypt results locally
  union mojov_memfmt_t x_check, max_check;
  simon_128_128_decrypt(&simon_state, x_enc, &x_check.ct);
  simon_128_128_decrypt(&simon_state, max_enc, &max_check.ct);

  libmin_printf("Final results:   x:%lu, max:%lu\n", x_check.pt.val, max_check.pt.val);


  // disable private register semantics (write 0)
  write_mprivregcfg(0);

  val = read_mprivregcfg();
  libmin_printf("After disable, mprivregcfg = 0x%lx, ", val);
  print_mprivregcfg(val);
  libmin_printf("\n");

  libmin_success();
  return 0;
}

