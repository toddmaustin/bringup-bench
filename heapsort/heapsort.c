#include "libmin.h"

static long bplong;
static long base[2048];

/*************************/
/*  Heap Sort Program    */
/*************************/

int
HSORT(long m, long p)
{
  long i,j,k,l;
  long size;

  long  msize, iran, ia, ic, im, ih, ir;
  long  count, ca, cb, cc, cd, ce, cf;

  msize = m * bplong;
  size  = m - 1;

  if (m >= 2048)
    {
      libmin_printf("base buffer overflow!\n");
      libmin_fail(1);
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
    libmin_printf("   %10ld\n", msize);

  /* check the array */
  for(i = 0; i < size-1; i++)
    {
      if (base[i] > base[i+1])
      {
        libmin_printf("ERROR: base array is not properly sorted!\n");
        libmin_fail(1);
      }
    }
  libmin_printf("INFO: base array is properly sorted!\n");

  return 0;
}

int
main(void)
{
  long  j, p;

  bplong = sizeof(long);

  libmin_printf("\n   Heap Sort C Program\n");
  libmin_printf("   Size of long (bytes): %ld\n\n", bplong);
  libmin_printf("   Array Size (bytes)\n");

				   /* Predetermine runtime (sec) for  */
				   /* memory size 2000 * sizeof(long),*/
				   /* and 256 iterations. p = 0 means */
				   /* don't print the result.         */
  j = 1024;
  p = 1;
  HSORT(j,p);

  return 0;
}
