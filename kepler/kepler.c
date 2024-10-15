/* Kepler.c:

   Solve Kepler's equation using various methods. Can be run as a standalone
   program or called via the entry point kepler (see below.)

   Kepler's equation is the transcendental equation

	E = M + e sin(E)

   where M is a given angle (in radians) and e is a number in the range
   [0,1). Solution of Kepler's equation is a key step in determining the
   position of a planet or satellite in its orbit. Here e is the eccentricity
   of the elliptical orbit and M is the "Mean anomaly." M is a fictitious
   angle that increases at a uniform rate from 0 at perihelion to 2Pi in
   one orbital period. E is an angle at the center of the ellipse. It is
   somewhat difficult to describe in words its precise geometric meaning --
   see any standard text on dynamical or positional astronomy, e.g.,
   W.E. Smart, Spherical Astronomy. Suffice it to say that E determines
   the angle v at the focus between the radius vector to the planet and
   the radius vector to the perihelion, the "true anomaly"  (the ultimate
   angle of interest.) Thus, kepler's equation provides the link between the
   "date", as measured by M, and the angular position of the planet on its
    orbit. 

   Compile: cc -o kepler kepler.c -lm

   ( Use the flag -D_SHORT_STRINGS if your compiler cannot handle multiline
     strings.)

   For usage information, give the command kepler -h or see USAGE defined
   below.

   API usage:

	extern int kepler(double *E, double M, double e, double derror,
            int method);

	where the last two parameters are a double specifying the 
        precision and an int specifying the method. The possible values
        of m are:

	m=0: simple iteration. 
	m=1: Newton's method.
	m=2: Binary search.
	m=3: Power series in e.
	m=4: Fourier Bessel series. 

        The answer is returned through the pointer passed as first argument.
	The function returns the number of iterations required or -1
	in case of an error.

   Also possibly of interest is the included routine for calculating bessel
   functions of the first kind with integer index, although I'm sure the
   implementation is pretty naive. The calling sequence is:

	extern double J(int n, double x);

   Compile this file with cc -c -DNO_MAIN and link your program with kepler.o.

   Author: Terry R. McConnell
	   trmcconn@syr.edu

*/

#include "libmin.h"

#define VERSION "1.11"

#ifndef PI
#define PI 3.14159265359
#endif
#define LAPLACE_LIMIT .6627434193
#define USAGE "kepler [-h -v -a <.nnnn...> -m <k>] M e"

#ifdef _SHORT_STRINGS
#define HELP USAGE
#else
#define HELP USAGE"\n\
-h: print this helpful message\n\
-v: print version number and exit\n\
-a: obtain solution to accuracy of  < .nnnn (default .0000001)\n\
-m: use selected calculation method k, where\n\
	k = 1: Simple iteration.\n\
	k = 2: Newton's method.\n\
	k = 3: Binary search.\n\
	k = 4: Series in powers of e. (e<.6627434193.)\n\
	k = 5: Fourier Bessel series.\n\
M = mean anomaly (radians)\n\
e = orbit eccentricty."
#endif

/* E = eccentric anomaly,
   e = eccentricity,
   M = mean anomaly
   (using radian measure).
*/

double bin_fact(int n, int j);  /* Used with e-series method. See below. */
double J(int,double);           /* Calculates Bessel function. */
static double derror = 0.000001;

/* All the algorithms for solving kepler's equation are implemented in
   the following subroutines. A subroutine is called iteratively from
   main, until the previous value of E differs from the current one by
   less than derror.

   To add a new method, add its implementation as a subroutine 
   with signature

	double foo(double E, double e, double M, int reset);

   It should return a better approximation to the true E
   given the current E and the values of e and M. When passed a 
   nonzero value for the reset parameter it should reinitialize any
   static information it retains and return.
   Then add the address of
   the new subroutine to the methods array defined below.
*/ 

/* CURRENTLY IMPLEMENTED METHODS: */

/* Used to solve kepler's equation by simple iteration */

double strict_iteration(double E, double e, double M, int reset)
{

	/* reset is not used in this routine. It may generate a compiler
           warning */
	return M + e*libmin_sin(E);
}

/* The following routine is used to solve kepler's equation using
   Newton's method. It is very fast and reliable for small values of
   e, but can be wildly erratic for e close to 1. See, e.g, the discussion
   in Jean Meeus, Astronomical Algorithms, Willmann-Bell, 1991, 181-193.
*/

double newton(double E, double e, double M, int reset)
{
	/* reset is not used in this routine. It may generate a compiler
           warning */
	return E + (M + e*libmin_sin(E) - E)/(1 - e*libmin_cos(E));
}

/* The following routine implements the binary search algorithm due
   to Roger Sinnott, Sky and Telescope, Vol 70, page 159 (August 1985.)
   It is not the fastest algorithm, but it is completely reliable. 
*/

double binary(double E, double e, double M, int reset)
{
	static double scale = .7853981633975;   /* PI/4 */
	double R;
	double X;

	if(reset) {
		scale = PI/4.0;
		return 0.0;
	}

	R = E - e*libmin_sin(E);
	X = M > R ? E + scale : E - scale;
	scale = scale/2.0;
	return X;
	
}

/* The following infinite series expansion for E in powers of e is known:

                 __
	        \       n
	E = M +      A e
		/__   n
		n=1

where              __ 
       	     n-1  \          k            (n-1)
A =      (1/2 n!)        (-1) C(n,k)(n-2k)   sin((n-2k)M),
 n                /__
                 0<= k <= [n/2]

which converges for e < LAPLACE_LIMIT (defined above). This is based upon
the Laplace inversion formula -- see the discussion of Burmann's theorem
and following material in Whittaker and Watson.

The bin_fact helper routine calculates C(n,k)(n-2k)^(n-1)/n!2^(n-1) */


double e_series(double E, double e, double M, int reset)
{
	static int n;
	int k;
	double n_2k,a_n=0.0,s_k;

	if(reset){
		n = 0;
		return 0.0;
	}

	if(n==0){
		n++;
		return M;
	}


	for(k=0;2*k<=n;k++){
		n_2k = (double)n - 2.0 * ((double)k);
		s_k = k%2 ? -1.0 : 1.0;   /*   (-1)^k */
		a_n += s_k*bin_fact(n,k)*libmin_sin(n_2k*M);
	}
	n++;
	return E + libmin_pow(e,n-1)*a_n;
}
	
/* The eccentric anomaly is an odd periodic function in the Mean Anomoly
   and so can be developed in a Fourier sine series. This turns out to
   be 
                 __
	        \  
	E = M +      (2/n)J (ne)sin(nM)
		/__        n 
		n=1

  where J_n is the Bessel function of the first kind. See, e.g, A Mathematical
  Introdution to Celestial Mechanics, Harry Pollard, Prentice Hall, 1966,
  pp 22-23. The following routine is used to sum this series.
*/


double j_series(double E, double e, double M, int reset)
{
	static int n;
	double dn, term;

	if(reset){
		n = 0;
		return 0.0;
	}

	if(n==0){
		n++;
		return M;
	}
	dn = (double)n;
	term = (2.0/(double)n)*J(n,dn*e)*libmin_sin(dn*M);
	n++;
	return E + term;
}

static void *methods[] = {strict_iteration,
					newton,
					binary,
					e_series,
					j_series,
				};

#define NMETHODS (sizeof methods /sizeof(void *))

/* Symbolic constants for method indices. */
#define IITERATION 0
#define INEWTON 1
#define IBINARY 2
#define IESERIES 3
#define IJSERIES 4

#ifndef NO_MAIN

int argc=5;
char *argv[] = { "kepler", "-m", "3", "0.34", "0.25" };

int
main(void)
{
	int n = 1,i=1;
	int m=1;
	double sign = 1.0;
	double M = 0.0, e = -0.1, E_old=PI/2 ,E;
	double (*method)(double,double, double,int);


	/* Process command line options */

	while(argv[i][0] == '-'){
		  if(libmin_strcmp(argv[i],"-h")==0){
			libmin_printf("%s\n", HELP);
			libmin_success();
		  }
		  if(libmin_strcmp(argv[i],"-v")==0){
			libmin_printf("%s\n",VERSION);
			libmin_success();
		  }
		  if(libmin_strcmp(argv[i],"-a")==0){
			derror = libmin_atof(argv[i+1]);
			if(derror <= DBL_EPSILON)
			        libmin_printf("Warning: requested precision may exceed implementation limit.\n");
			i += 2;
			continue;
		  }
		  if(libmin_strcmp(argv[i],"-m")==0){
			m = libmin_atoi(argv[i+1]);
			if((m<=0) || (m>NMETHODS)){
				libmin_printf("Bad method number %d\n",m);
				return 1;
			}
			i += 2;
			continue;
		  }
		  libmin_printf("kepler: Unknown option %s\n", argv[i]);
		  libmin_printf("%s\n",USAGE);
		  return 1;
		}
	if(i + 2 > argc){
		libmin_printf("%s\n",USAGE);
		return 1;
	}
	M = libmin_atof(argv[i++]);
	e = libmin_atof(argv[i]);
	method = (double(*)(double,double,double,int))methods[m-1];

	if((m==4)&&(e > LAPLACE_LIMIT)){
		libmin_printf("e cannot exceed %f for this method.\n",
				LAPLACE_LIMIT);
		return 1;
	}

	if((e<0)||(e>=1.0)){
		libmin_printf("Eccentricity %f out of range.\n",e);
		return 1;
	}

	/* Normalize M to lie between 0 and PI */
	sign = M > 0 ? 1.0 : -1.0;
	M = libmin_fabs(M)/(2*PI);
	M = (M - libmin_floor(M))*2*PI*sign;
	sign = 1.0;
	if(M > PI){
		M = 2*PI - M;
		sign = -1.0;
	}
	
	/* Do selected calculation, and quit when accuracy is bettered. */
	while(libmin_fabs(E_old - (E = method(E_old,e,M,0))) >= derror){
		E_old = E;
		libmin_printf("n = %d\tE = %f\n",n++,sign*E);
	}

  libmin_success();
	return 0;
}
#endif /* NO_MAIN */

/* The bin_fact routine calculates C(n,k)(n-2k)^(n-1)/n!2^(n-1). This
   routine assumes 2k <  n, and tries to keep the intermediate products
   small to prevent overflow.  */

double 
bin_fact(int n, int k)
{
		int j;
		double cum_prod = 1.0;
		double num_fact,den_fact,dj,dk,x;

		x = ((double) n)/2.0 - (double)k;

		for(j=n-k;j>1;j--){
			dj = (double)j;
			dk = (double) n -(double)k - dj + 1.0;
			den_fact = n - k - j + 1 <= k ? dk*dj : dj;  
			num_fact = n - k - j + 1 <= k ? x*x : x; 
			cum_prod *= (num_fact/den_fact);
		}
		return cum_prod;
}

/* The following routine calculates the Bessel function of the first kind 
   for an integer index. We just sum the series representation given by


                      __                     2j
	              \        j        (x/2)
J (x) = 1/n! (x/2)^n       (-1)   __________________
 n	              /__          j!(n+1)...(n+j)
		      j=0

   
See Special functions and their applications, N.N. Lebedev, Dover, 1972,
pp 95-142 for an introduction to Bessel functions and related cylinder
functions.

*/

double J(int n, double x)
{
	double dsum=0.0,dterm,s_j,d_n,d_j,cfact=1.0;
	int j,nn;

	nn = n >= 0 ? n : -n;  /* Absolute value of n. Use the relation
                                  J  (x) = (-1)^n J  (x) for negative n 
				    -n              n    */

	d_n = (double) nn;
	
	/* Calculate the common factor (x/2)^n/n! so it only has to be
           done once. */

	for(j=1;j<=nn;j++){
		d_j = (double)j;
		cfact *= x/(2.0*d_j);
	}

	/* j = 0 term: */
	dsum = dterm = cfact;

	j = 1;

	do {
		d_j = (double)j;
		s_j = j%2 ? -1.0: 1.0;
		dterm *= x*x/(d_j*4.0*(d_n + d_j));
		dsum += s_j*dterm;
		j++;
	} while( dterm > DBL_EPSILON );
		
	s_j = nn%2 ? -1.0 : 1.0;
	return  n >= 0 ? dsum : s_j*dsum;
}

int
kepler(double *E, double M, double e, double my_derror, int m)
{
	int count = 0;
	double sign = 1.0;
	double  E_old=PI/2;
	double (*method)(double,double, double,int);
	
	if((m<0) || (m>=NMETHODS))return -1;

	method = (double(*)(double,double,double,int))methods[m];

	if((m==3)&&(e > LAPLACE_LIMIT))
		return -1;

	if((e<0)||(e>=1.0))
		return -1;

	/* Normalize M to lie between 0 and PI */
	sign = M > 0 ? 1.0 : -1.0;
	M = libmin_fabs(M)/(2*PI);
	M = (M - libmin_floor(M))*2*PI*sign;
	sign = M > 0 ? 1.0 : -1.0;
	M = libmin_fabs(M);
	if(M > PI){
		M = 2*PI - M;
		sign = -1.0;
	}
	
	/* Do selected calculation, and quit when accuracy is bettered. */
	while(libmin_fabs(E_old - (*E = method(E_old,e,M,0))) >= my_derror){
		E_old = *E;
		count++;
	}
	*E = sign*(*E);
	method(0.0,0.0,0.0,1);  /* reset */

	return count;
}

