#include "libmin.h"

static uint64_t keytext[4];
static uint64_t plaintext[2];
static uint64_t ciphertext[2];
static uint64_t newplain[2];

static inline uint64_t
mask32(uint64_t x)
{
  return x & 0xFFFFFFFFul;
}

static void
encipher(const uint64_t in[2], uint64_t out[2], const uint64_t key[4])
{
  uint64_t y = in[0], z = in[1], sum = 0;
  const uint64_t delta = 0x9E3779B9ul;
  uint64_t a = key[0], b = key[1], c = key[2], d = key[3];

  for (unsigned n = 0; n < 32; ++n)
  {
    sum = mask32(sum + delta);
    y = mask32(y + (((z << 4) + a) ^ (z + sum) ^ ((z >> 5) + b)));
    z = mask32(z + (((y << 4) + c) ^ (y + sum) ^ ((y >> 5) + d)));
  }

  out[0] = y;
  out[1] = z;
}

static void
decipher(const uint64_t in[2], uint64_t out[2], const uint64_t key[4])
{
  uint64_t y = in[0], z = in[1], sum = 0xC6EF3720ul;
  const uint64_t delta = 0x9E3779B9ul;
  uint64_t a = key[0], b = key[1], c = key[2], d = key[3];

  for (unsigned n = 0; n < 32; ++n)
  {
    z = mask32(z - (((y << 4) + c) ^ (y + sum) ^ ((y >> 5) + d)));
    y = mask32(y - (((z << 4) + a) ^ (z + sum) ^ ((z >> 5) + b)));
    sum = mask32(sum - delta);
  }

  out[0] = y;
  out[1] = z;
}

int
main(void)
{
  const uint64_t keytext_plain[4] = {
    358852050ul, 311606025ul, 739108171ul, 861449956ul
  };
  const uint64_t plaintext_plain[2] = { 765625614ul, 14247501ul };
  const uint64_t cipherref[2] = { 0x9fe2c864ul, 0xd7da4da4ul };

  for (unsigned i = 0; i < 4; ++i)
    keytext[i] = keytext_plain[i];
  for (unsigned i = 0; i < 2; ++i)
    plaintext[i] = plaintext_plain[i];

  encipher(plaintext, ciphertext, keytext);

  if (ciphertext[0] != cipherref[0] || ciphertext[1] != cipherref[1])
  {
    libmin_printf("ERROR: TEA ciphertext mismatch\n");
    return -1;
  }

  decipher(ciphertext, newplain, keytext);

  if (newplain[0] != plaintext_plain[0] || newplain[1] != plaintext_plain[1])
  {
    libmin_printf("ERROR: TEA decipher mismatch\n");
    return -1;
  }

  libmin_printf("TEA Cipher results:\n");
  libmin_printf("  plaintext:  0x%08lx 0x%08lx\n", plaintext[0], plaintext[1]);
  libmin_printf("  ciphertext: 0x%08lx 0x%08lx\n", ciphertext[0], ciphertext[1]);
  libmin_printf("  newplain:   0x%08lx 0x%08lx\n", newplain[0], newplain[1]);

  libmin_success();
  return 0;
}
