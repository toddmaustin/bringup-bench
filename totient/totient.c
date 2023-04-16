/* totient.c: Calculates the Euler totient function, phi. 
 *
 *  By Terry R. McConnell
 *
 *                              Theory
 *
 *  Euler's totient function, phi(n), is defined as the number of natural
 *  numbers <= n which are relatively prime to n. (We count 1 as relatively
 *  prime to everything.) It is an extremely important function in number
 *  theory. For primes p it is clear from the definition that phi(p) = p - 1.
 *  For powers of a prime it also easy to see (use induction on n) that
 *  phi(p^n) = p^(n-1)(p-1). Thus, e.g, phi(125) = 100. For all other
 *  values phi can be computed by factoring n completely and using the
 *  following result:
 *
 *  Theorem: Phi is a multiplicative function, i.e., if (m,n) = 1 (relatively
 *  prime) then phi(mn) = phi(m)phi(n).
 *
 *  Perhaps the easiest proof is via group theory. It follows from the
 *  Chinese Remainder Theorem that the multiplicative group of invertible 
 *  integers modulo  mn, Z(mn), is isomorphic to the direct product Z(m)xZ(n).
 *  The result follows since the orders of these groups are given by the 
 *  totient function. 
 *
 *  For a proof of the version of the Chinese Remainder Theorem required,
 *  see Theorem 3.7 in H.M. Stark, An Introduction to Number Theory, 
 *  Markham, Chicago, 1970.
 *
 *  Our implementation is highly recursive and is motivated by the McCarthy
 *  conditional statement formalism. (See Marvin Minsky, Computation:
 *  finite and infinite machines, Prentice Hall, Englewood Cliffs, 1967,
 *  problem 10.7-1.)
 *
 *  We define phi(-n) = phi(n), and do not define phi(0).
 *
*/

/* compile: cc -o totient  totient.c

      Use -D_SHORT_STRINGS if your compiler does not support multiline
          string constants.

   Run totient -h for usage information.

*/


#include "libmin.h"

#define VERSION "1.0"
#define USAGE "totient [ -h -v -- ] n"
#ifndef _SHORT_STRINGS
#define HELP "\ntotient [ -h -v --] n\n\n\
Find the Euler totient function of n, the number of k <= n such that\n\
k and n are relatively prime. (1 is relatively prime to everything.)\n\n\
--: Signal end of options so that negative n can be input. (Silly, since\n\
    we merely define phi(-n) = phi(n).)\n\
-v: Print version number and exit. \n\
-h: Print this helpful information. \n\n"
#else
#define HELP USAGE
#endif

/* my_gcd: return the greatest common divisor of a and b, or -1 if it
 * is not defined. (See also euclid.c for a standalone implementation
 * of my_gcd.) 
 * 
 */

static int my_gcd(int a, int b)
{
	int q,r,t;

	/* Normalize so that 0 < a <= b */
	if((a == 0)||(b == 0)) return -1;
	if(a < 0) a = -a;
	if(b < 0) b = -b;
	if(b < a){
		t = b;
		b = a;
		a = t;
	}

	/* Now a <= b and both >= 1. */

	q = b/a;
	r = b - a*q;
	if(r == 0) 
		return a;

	return my_gcd(a,r);
}

/* In conditional statement form, phi can be defined together with another
 * function of 2 variables we denote as phiphi. We have phi(n) = phiphi(n,2)
 * and phiphi(y,x) = if(x = y-1 then x else if( x|y then
 * 	if((x,y/x)=1 then phi(x)phi(y/x) else x phi(y/x)) else phi(y,x+1))))
 */

static int phiphi(int,int);
static int phi(int n)
{
	if(n<0)n=-n;
	/* handle a few trivial boundary cases */
	if(n<=1)return 0;
	if(n==2)return 1;
	if(n==3)return 2;
	return phiphi(n,2);
}

/* This only gets called with y >= 3 and y > x >= 2 */

static int phiphi(int y, int x)
{
	int z;

	if(x+1 == y)return x; /* phi(prime p) = p-1 */
	if((y%x)==0){
		if(my_gcd(x,z=y/x)==1)
			return phi(x)*phi(z); /* multiplicative property */
		else
			return x*phi(z); /* This is a tricky case. It may
					    happen when x is a prime such
					    that a power of x divides y. In
					    case y = p^n, phi(y) = p^(n-1)(p-1)
					   */
	}
	else return phiphi(y,x+1);
}

int
main(void)
{
	int argc=1; char **argv=NULL;
	int n;
	int j=0;

	/* Process command line */
	while(++j < argc){
		if(argv[j][0] == '-')
			switch(argv[j][1]){ 
				case '-':
					++j;
					break;
				case 'v':
				case 'V':
					libmin_printf("%s\n",VERSION);
					libmin_success();
				case '?':
				case 'h':
				case 'H':
					libmin_printf("%s\n",HELP);
					libmin_success();
				default:
					libmin_printf("totient: unkown option %s\n", argv[j]);
					libmin_printf("%s\n",USAGE);
					libmin_fail(1);
			}
		break;
	}

	if(j >= argc){
		//fprintf(stderr,"totient: usage error.\n");
		//fprintf(stderr,"%s\n",USAGE);
		n = 45457;
	}
	else
	  n = libmin_atoi(argv[j++]);

	if(n == 0){
		libmin_printf("totient: not defined for n = 0.\n");
		return 1;
	}

	libmin_printf("phi(%d) = %d\n",n,phi(n));

  libmin_success();
	return 0;
}

