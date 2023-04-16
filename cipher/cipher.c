#include "libmin.h"

void
encipher(unsigned int *const in,
	 unsigned int *const out,
	 const unsigned int *const key)
{
  unsigned int y=in[0], z=in[1], sum=0, delta=0x9E3779B9;
  unsigned int a=key[0], b=key[1], c=key[2], d=key[3], n=32;

  while (n-->0)
    {
      sum += delta;
      y += ((z << 4)+a) ^ (z+sum) ^ ((z >> 5)+b);
      z += ((y << 4)+c) ^ (y+sum) ^ ((y >> 5)+d);
    }
  out[0]=y; out[1]=z;
}

void
decipher(unsigned int *const in,
	 unsigned int *const out,
	 const unsigned int *const key)
{
  unsigned int y=in[0], z=in[1], sum=0xC6EF3720, delta=0x9E3779B9;
  unsigned int a=key[0], b=key[1], c=key[2], d=key[3], n=32;

  /* sum = delta<<5, in general sum = delta * n */
  while (n-->0)
    {
      z -= ((y << 4)+c) ^ (y+sum) ^ ((y >> 5)+d);
      y -= ((z << 4)+a) ^ (z+sum) ^ ((z >> 5)+b);
      sum -= delta;
    }
  out[0]=y; out[1]=z;
}

unsigned int keytext[4] = { 358852050,	311606025, 739108171, 861449956 };
unsigned int plaintext[2] = { 765625614, 14247501 };
unsigned int cipherref[2] = { 0x9fe2c864, 0xd7da4da4 };
unsigned int ciphertext[2];
unsigned int newplain[2];

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
  libmin_printf("  plaintext:  0x%08lx 0x%08lx\n",
	       plaintext[0], plaintext[1]);
  libmin_printf("  ciphertext: 0x%08lx 0x%08lx\n",
	       ciphertext[0], ciphertext[1]);
  libmin_printf("  newplain:   0x%08lx 0x%08lx\n",
	       newplain[0], newplain[1]);

  libmin_success();
  return 0;
}

