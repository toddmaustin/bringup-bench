#include "libmin.h"
#include "simon.h"

typedef unsigned __int128 uint128_t;

#define SECRET

extern inline uint64_t
__instret(void)
{
  uint64_t insts;
  __asm__ volatile ("rdinstret %0" : "=r"(insts));

  return insts;
}

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

SECRET int
secret_cmov(SECRET bool p, SECRET int x, SECRET int y)
{
  return (int)p*x + (int)!p*y;
}

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

inline extern uint64_t
secret_decrypt(uint128_t ct)
{
  union mojov_memfmt_t mempkt;
  simon_128_128_decrypt(&simon_state, ct, &mempkt.ct);
  return mempkt.pt.val;
}

// supported sizes: 256 (default), 512, 1024, 2048
#define DATASET_SIZE 256
uint64_t raw_data[DATASET_SIZE];
SECRET uint128_t secret_data[DATASET_SIZE];

// total swaps executed so far
uint128_t swaps;

void
print_data(uint64_t *data, unsigned size)
{
  libmin_printf("DATA DUMP:\n");
  for (unsigned i=0; i < size; i++)
    libmin_printf("  data[%u] = %ld\n", i, data[i]);
}

void
bubblesort(uint128_t *data, unsigned size)
{
  for (unsigned i=0; i < size-1; i++)
  {
    for (unsigned j=0; j < size-1; j++)
    {
      // swap needed?
      __asm__ volatile (
        // SECRET bool swap = (data[j] > data[j+1]);
        LDE(  t3, %0, 0) // data[j]
        LDE(  t4, %1, 0) // data[j+1]
        "slt  t5, t4, t3\n\t" // swap
        // perform the swap
        // data[j] = secret_cmov(swap, data[j+1], data[j]);
        "czero.eqz t6, t4, t5\n\t"
        "czero.nez t5, t3, t5\n\t"
        "or        t6, t5, t6\n\t"
        SDE(       t6, %0, 0)
        // data[j+1] = secret_cmov(swap, tmp, data[j+1]);
        "slt  t5, t4, t3\n\t" // swap
        "czero.eqz t6, t3, t5\n\t"
        "czero.nez t3, t4, t5\n\t"
        "or        t6, t3, t6\n\t" 
        SDE(       t6, %1, 0)
        // count the number of swaps executed
        // swaps = secret_cmov(swap, swaps+1, swaps);
        LDE(  t3, %2, 0) // swaps
        "add t4, t3, t5\n\t" // swaps+1
        // "czero.eqz t6, t3, t5\n\t"
        // "czero.nez t5, t4, t5\n\t"
        // "or        t6, t5, t6\n\t"
        SDE(  t4, %2, 0) // swaps
        :
        : "r" (&data[j]), "r" (&data[j+1]), "r" (&swaps)
        : "t3", "t4", "t5", "t6" // clobbered registers
      );


    }
  }
}

int
main(void)
{
  // initilize cipher engine, for checking results
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

  // initialize the pseudo-RNG
  libmin_srand(42);

  // initialize swaps
  // swaps = 0;
  __asm__ volatile (
    "mv   t3, x0\n\t"
    SDE  (t3, %0, 0)
    :
    : "r" (&swaps)
    : "t3" // clobbered registers
  );

  // initialize the array to sort
  for (unsigned i=0; i < DATASET_SIZE; i++)
  {
    raw_data[i] = libmin_rand();
    __asm__ volatile (
      // secret_data[i] = raw_data[i];
      "ld   t3, (%0)\n\t"
      SDE(  t3, %1, 0)
      :
      : "r" (&raw_data[i]), "r" (&secret_data[i])
      : "t3" // clobbered registers
    );
  }
  print_data(raw_data, DATASET_SIZE);

  {
    // performance monitoring
    uint64_t icnt_start = __instret();

    bubblesort(secret_data, DATASET_SIZE);

    uint64_t icnt_end = __instret();
    libmin_printf("INFO: bubblesort inst count = %lu.\n", icnt_end - icnt_start + 1);
  }


  // decrypt the array
  for (unsigned i=0; i < DATASET_SIZE; i++)
    raw_data[i] = secret_decrypt(secret_data[i]);
  print_data(raw_data, DATASET_SIZE);

  // check the array
  for (unsigned i=0; i < DATASET_SIZE-1; i++)
  {
    if (raw_data[i] > raw_data[i+1])
    {
      libmin_printf("ERROR: data is not properly sorted.\n");
      return -1;
    }
  }
  libmin_printf("INFO: %lu swaps executed.\n", secret_decrypt(swaps));
  libmin_printf("INFO: data is properly sorted.\n");

  libmin_success();
  return 0;
}
