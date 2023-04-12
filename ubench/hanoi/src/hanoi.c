#include "libcosim.h"

#define other(i,j) (6-(i+j))

static int num[4];
static long count;

static int
mov(int n, int f, int t)
{
  int o;

  if (n == 1)
    {
      num[f]--;
      num[t]++;
      count++;
      return 0;
    }
  o = other(f, t);
  mov(n-1, f, o);
  mov(1, f, t);
  mov(n-1, o, t);
  return 0;
}

int
newmain(void)
{
  int disk, Loops = 0;

  cosim_printf("Towers of Hanoi Puzzle Test Program\n");
  cosim_printf("Disks     Moves\n");

  disk = 0;

  while (1)
    {
      disk++;
      num[0] = 0;
      num[1] = disk;
      num[2] = 0;
      num[3] = 0;
      count  = 0;

      mov(disk,1,3);

      Loops = Loops + 1;
      cosim_printf("%3d  %10ld\n",disk,count);

      if ( disk == 5 ) break;
    }
  return 0;
}
