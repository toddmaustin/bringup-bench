#include "libmin.h"

void
encipher(uint32_t *const in,
	 uint32_t *const out,
	 const uint32_t *const key)
{
  uint32_t y=in[0], z=in[1], sum=0, delta=0x9E3779B9;
  uint32_t a=key[0], b=key[1], c=key[2], d=key[3], n=32;

  while (n-->0)
    {
      sum += delta;
      y += ((z << 4)+a) ^ (z+sum) ^ ((z >> 5)+b);
      z += ((y << 4)+c) ^ (y+sum) ^ ((y >> 5)+d);
    }
  out[0]=y; out[1]=z;
}

void
decipher(uint32_t *const in,
	 uint32_t *const out,
	 const uint32_t *const key)
{
  uint32_t y=in[0], z=in[1], sum=0xC6EF3720, delta=0x9E3779B9;
  uint32_t a=key[0], b=key[1], c=key[2], d=key[3], n=32;

  /* sum = delta<<5, in general sum = delta * n */
  while (n-->0)
    {
      z -= ((y << 4)+c) ^ (y+sum) ^ ((y >> 5)+d);
      y -= ((z << 4)+a) ^ (z+sum) ^ ((z >> 5)+b);
      sum -= delta;
    }
  out[0]=y; out[1]=z;
}

uint32_t keytext[4] = { 358852050,	311606025, 739108171, 861449956 };
uint32_t plaintext[2] = { 765625614, 14247501 };
uint32_t cipherref[2] = { 0x9fe2c864, 0xd7da4da4 };
uint32_t ciphertext[2];
uint32_t newplain[2];

int
main(void)
{

  encipher(plaintext, ciphertext, keytext);
  if (ciphertext[0] != cipherref[0] || ciphertext[1] != cipherref[1])
    libmin_fail(1);
  decipher(ciphertext, newplain, keytext);
  if (newplain[0] != plaintext[0] || newplain[1] != plaintext[1])
    libmin_fail(2);
  
  libmin_printf("TEA Cipher results:\n");
  libmin_printf("  plaintext:  0x%08x 0x%08x\n", plaintext[0], plaintext[1]);
  libmin_printf("  ciphertext: 0x%08x 0x%08x\n", ciphertext[0], ciphertext[1]);
  libmin_printf("  newplain:   0x%08x 0x%08x\n", newplain[0], newplain[1]);

  libmin_success();
  return 0;
}

