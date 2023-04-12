#include <stdlib.h>
#include "libcosim.h"

static long bplong;
static long base[2048];

/*************************/
/*  Heap Sort Program    */
/*************************/

static int HSORT(m,p)
long m,p;
{
  register long i,j,k,l;
  register long size;

  long  msize, iran, ia, ic, im, ih, ir;
  long  count, ca, cb, cc, cd, ce, cf;

  msize = m * bplong;
  size  = m - 1;

  if (m >= 2048)
    {
      cosim_printf("base buffer overflow!\n");
      cosim_fail(1);
    }

  ia = 106;
  ic = 1283;
  im = 6075;
  ih = 1001;

  count = 0;

  iran = 47;                        /* Fill with 'random' numbers */
  for(i=1 ; i<=size ; i++)
    {
      iran = (iran * ia + ic) % im;
      *(base+i) = 1 + (ih * iran) / im;
    }

  k = (size >> 1) + 1;              /* Heap sort the array */
  l = size;
  ca = 0; cb = 0; cc = 0;
  cd = 0; ce = 0; cf = 0;

  for (;;)
    {
      ca++;
      if (k > 1)
	{
	  cb++;
	  ir = *(base+(--k));
	}
      else
	{
	  cc++;
	  ir = *(base+l);
	  *(base+l) = *(base+1);
	  if (--l == 1)
	    {
	      *(base+1) = ir;
	      goto Done;
	    }
	}

      i = k;
      j = k << 1;

      while (j <= l)
	{
	  cd++;
	  if ( (j < l) && (*(base+j) < *(base+j+1)) ) ++j;
	  if (ir < *(base+j))
	    {
	      ce++;
	      *(base+i) = *(base+j);
	      j += (i=j);
	    }
	  else
	    {
	      cf++;
	      j = l + 1;
	    }
	}
      *(base+i) = ir;
    }
 Done:
  count = count + ca;

  /* Scale runtime per iteration */
  ir = count;
  ir = (ir + ca) / 2;

  if ( p != 0L )
    cosim_printf("   %10ld\n", msize);

  return 0;
}

int
newmain(void)
{
  long  j, p;

  bplong = sizeof(long);

  cosim_printf("\n   Heap Sort C Program\n");
  cosim_printf("   Version 1.0, 04 Oct 1992\n\n");
  cosim_printf("   Size of long (bytes): %ld\n\n", bplong);
  cosim_printf("   Array Size\n");
  cosim_printf("    (bytes)\n");

				   /* Predetermine runtime (sec) for  */
				   /* memory size 2000 * sizeof(long),*/
				   /* and 256 iterations. p = 0 means */
				   /* don't print the result.         */
  j = 50;
  p = 1;
  HSORT(j,p);

  return 0;
}
