#include "libmin.h"

/**
 * `murmurhash.h' - murmurhash
 *
 * copyright (c) 2014-2022 joseph werle <joseph.werle@gmail.com>
 */

/**
 * Returns a murmur hash of `key' based on `seed'
 * using the MurmurHash3 algorithm
 */

uint32_t murmurhash(const char *, uint32_t, uint32_t);

/**
 * `murmurhash.h' - murmurhash
 *
 * copyright (c) 2014-2022 joseph werle <joseph.werle@gmail.com>
 */

uint32_t
murmurhash (const char *key, uint32_t len, uint32_t seed)
{
  uint32_t c1 = 0xcc9e2d51;
  uint32_t c2 = 0x1b873593;
  uint32_t r1 = 15;
  uint32_t r2 = 13;
  uint32_t m = 5;
  uint32_t n = 0xe6546b64;
  uint32_t h = 0;
  uint32_t k = 0;
  uint8_t *d = (uint8_t *) key; // 32 bit extract from `key'
  const uint32_t *chunks = NULL;
  const uint8_t *tail = NULL; // tail - last 8 bytes
  int i = 0;
  int l = len / 4; // chunk length

  h = seed;

  chunks = (const uint32_t *) (d + l * 4); // body
  tail = (const uint8_t *) (d + l * 4); // last 8 byte chunk of `key'

  // for each 4 byte chunk of `key'
  for (i = -l; i != 0; ++i) {
    // next 4 byte chunk of `key'
    k = chunks[i];

    // encode next 4 byte chunk of `key'
    k *= c1;
    k = (k << r1) | (k >> (32 - r1));
    k *= c2;

    // append to hash
    h ^= k;
    h = (h << r2) | (h >> (32 - r2));
    h = h * m + n;
  }

  k = 0;

  // remainder
  switch (len & 3) { // `len % 4'
    case 3: k ^= (tail[2] << 16);
    case 2: k ^= (tail[1] << 8);

    case 1:
      k ^= tail[0];
      k *= c1;
      k = (k << r1) | (k >> (32 - r1));
      k *= c2;
      h ^= k;
  }

  h ^= len;

  h ^= (h >> 16);
  h *= 0x85ebca6b;
  h ^= (h >> 13);
  h *= 0xc2b2ae35;
  h ^= (h >> 16);

  return h;
}

int
main(void)
{
  uint32_t seed = 0;

  libtarg_start_perf();
  const char *key_1 = "kinkajou"; // 0xb6d99cf8
  uint32_t hash_1 = murmurhash(key_1, (uint32_t)libmin_strlen(key_1), seed);

  const char *key_2 = "The bringup-bench benchmark MURMUR made this.";
  uint32_t hash_2 = murmurhash(key_2, (uint32_t)libmin_strlen(key_2), seed);

  const char *key_3 = "It has to start somewhere, it has to start sometime, what better place than "
                      "here? What better time than now?";
  uint32_t hash_3 = murmurhash(key_3, (uint32_t)libmin_strlen(key_3), seed);
  libtarg_stop_perf();

  libmin_printf("murmurhash(\"%s\") = 0x%x\n", key_1, hash_1);
  libmin_printf("murmurhash(\"%s\") = 0x%x\n", key_2, hash_2);
  libmin_printf("murmurhash(\"%s\") = 0x%x\n", key_3, hash_3);

  libmin_success();
  return 0;
}
