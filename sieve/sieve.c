#include "libmin.h"

		     /***********************************************/
#define LIMIT 8      /* You may need to change this to '3' for PC's */
		     /* and Clones or you can experiment with higher*/
		     /* values, but '13' is currently the max.      */
		     /***********************************************/
static long L_Prime,N_Prime;      /* Last Prime and Number of Primes Found */

static char flags[8192];

void err(char* s);

/**************************************/
/*  Sieve of Erathosthenes Program    */
/**************************************/

static int
SIEVE(long m, long p)
{
  register long i,prime=0,k;
  register long count,size;
  long j;

  size  = m - 1;

  N_Prime   = 0L;
  L_Prime   = 0L;

  j = 0;

      count = 0;

      for(i=0 ; i<=size ; i++)
	{
	  *(flags+i) = TRUE;                                /* 1*size  */
	}                                                 /* 3*size  */
      
      for(i=0 ; i<=size ; i++)
	{
	  if(*(flags+i))                                /* 2*size  */
	    {                                             /* 1*count */
	      count++;                                      /* 1*count */
	      prime = i + i + 3;                            /* 3*count */
	      for(k = i + prime ; k<=size ; k+=prime)     /* 3*count */
		{
		  *(flags+k)=FALSE;                           /* 1*ci    */
		}                                           /* 3*ci    */
	      /* 1*count */
	    }
	}                                               /* 3*size  */
      
      j = j + count;
						
  N_Prime = j ;
  L_Prime = prime;

  if (p != 0L)
    libmin_printf("  %9ld   %8ld     %8ld\n",m,N_Prime,L_Prime);

  return 0;
}

int
main(void)
{

  long  j,p;

  libmin_printf("\n   Sieve of Eratosthenes (Scaled to 10 Iterations)\n");
  libmin_printf("   Version 1.2b, 26 Sep 1992\n\n");
  libmin_printf("   Array Size   Number   Last Prime\n");
  libmin_printf("    (Bytes)   of Primes\n");
	
  j = 1024;
  p = 1;
  SIEVE(j,p);

  libmin_success();
  return 0;
}


