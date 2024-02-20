#include "libmin.h"

/* BUILD : g++ FisherYatesShuffle.cpp -std=c++11*/

static int
rand_int(int n)
{
  int limit = RAND_MAX - RAND_MAX % n;
  int rnd;

  do {
    rnd = libmin_rand();
  } 
  while (rnd >= limit);
  return rnd % n;
}

void
fy_shuffle(int *array, int n)
{
  int i, j, tmp;

  for (i = n - 1; i > 0; i--)
  {
    j = rand_int(i + 1);
    libmin_assert(i >= 0 && i < n && j >= 0 && j < n);
    tmp = array[j];
    array[j] = array[i];
    array[i] = tmp;
  }
}

void
print(const char *prefix, int *array, int n)
{
  libmin_printf("%s", prefix);
  for (int i = 0; i < n; i++)
    libmin_printf("%2d ", array[i]);
  libmin_printf("\n");
}

#define SZ_A 11
int a[SZ_A] = {0,1,2,3,4,5,6,7,8,9,10};

#define SZ_B 19
int b[SZ_B] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18};

int
main(void)
{
  /* initialize random seed: */
  libmin_srand(42);

  for (int k = 0; k < 8; k++)
  {
    print("A (before): ", a, SZ_A);
    fy_shuffle(a, SZ_A);
    print("A (after):  ", a, SZ_A);

    print("B (before): ", b, SZ_B);
    fy_shuffle(b, SZ_B);
    print("B (after):  ", b, SZ_B);
  }
  
  libmin_success();
  return 0;
}
