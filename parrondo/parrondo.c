/* parrando.c: simulation of J. Parrando's probability paradox */


/* Parrondo's game is based upon two simple games of chance.

        The gambler's fortune starts at 0.

	The simple game: Toss a biased coin and win +1 with probability
		S_WIN_PROB (defined below). Otherwise win -1; 

	The complex game: If the player's fortune is divisible by 3, toss
		the "bad coin" having win probability BAD_WIN_PROB.
		If the player's fortune is not divisible by 3 toss the
                "good coin" having win probability GOOD_WIN_PROB.

	A game ends when the accumlated fortune exceeds MAX_FORTUNE ( a "win" )
	or dips below -MAX_FORTUNE ( a "loss .)  

	The numbers are chosen so that each game is quite unfavorable.
        Remarkably, when the games are alternated at random, the resulting
        game is quite favorable.

	This program simulates many trials of a Parrondo game and reports
        statistics on their outcomes. (It can also simulate the simple and
        complex games individually.) The number of trials, fortune limits,
        and a seed for the random number can be supplied on the command line.
        The -h option prints detailed help. 

	For more information on Parrondo games and related phenomena see
        J. Parrondo's website, http://seneca.fis.ucm.es/parr/,  or the
        announcement in Nature magazine, 23/30, December 1999.
*/

/* compile: cc -o parrondo parrondo.c 

      Use -D_NO_RANDOM if your library doesn't have random/srandom. Most do,
       	but the only truly portable RNG is rand/srand. Unfortunately it has
        very poor performance, so you should use random if possible.

      Use -D_MAX_RAND=  to set the size of the maximum value returned by
         random(). The portable RNG rand() always returns a maximum of 
         RAND_MAX (defined in stdlib.h), but some implementations of random
         do not use this value. Read the man page for random to be sure. A
	 common value is 2^31-1 = 2147483647. In so, and this is not the
         value of RAND_MAX on your system, you would compile with
         -D_MAX_RAND=214748367.

      Use -D_SHORT_STRINGS if your compiler does not support multiline
          string constants.
*/


#include "libmin.h"

#define VERSION "1.1"
#define USAGE "parrondo [ -s number -t number -m number -1 -2 -h -v]"
#ifndef _SHORT_STRINGS
#define HELP "parrondo [ -s number -t number -m number -1 -2 ]\n\n\
Print information on simulations of Parrondo's paradoxical game.\n\n\
-s: Use next argument as RNG seed. (otherwise use system time as seed.)\n\
-t: Use next argument as number of trials. Default 10000.\n\
-m: Use number as max fortune (win), -number as min fortune(loss). Default 50. \n\
-v: Print version number and exit. \n\
-h: Print this helpful information. \n\
-1: Simulate simple game alone.\n\
-2: Simulate complex game alone.\n\n"
#else
#define HELP USAGE
#endif

/* Default values */
#define MAX_FORTUNE 50
#define MAX_ITERATIONS 1000000L
#define TRIALS 1000
#define INITIAL_SEED 3445
#ifndef _MAX_RAND
#define _MAX_RAND RAND_MAX
#endif

/* See above for meaning of these */
#define S_WIN_PROB .495
#define BAD_WIN_PROB .095
#define GOOD_WIN_PROB .745


/* return -1 or +1 according as a simulated coin toss is heads (+1) or
   tails (-1). Take p as probability of heads.
*/

int
cointoss(double p)
{

	double U;   /* U(0,1) random variable */

	U = ((double)libmin_rand())/((double)_MAX_RAND);
	return U >= p ? -1 : 1;
} 

/* One play of the simple game: +1 if win, -1 if loss. */

int play_s(void)
{
	return cointoss(S_WIN_PROB);
		
}

/* One play of the complicated game: +1 if win, -1 if loss. */

int play_c(int fortune)
{

	if( fortune % 3 )
		return cointoss(GOOD_WIN_PROB);
	return cointoss(BAD_WIN_PROB);
}
	


int argc=6;
char *argv[] = { "parrondo", "-2", "-t", "10", "-S", "1" };

int
main(void)
{
	long n=0L;
	double n_bar,n_tot=0.0;
	int trials = TRIALS;
	int i=0,j=0,m;
	int win_count = 0;
	int loss_count = 0;
	long site_visits[3];  /* counts visits to numbers mod 3 */
	int fortune = 0;
	int max_fortune = MAX_FORTUNE;
	double game_select = 0.5;  /* Governs a coin toss below which selects
                                      between games. Setting this to 1.0 chooses
				      complex game only. Setting to 0.0 chooses
                                      simple game only. 
				   */
                                      
	long seed=0;

	/* Process command line */
	while(++j < argc){
		if(argv[j][0] == '-')
			switch(argv[j][1]){ 
				case 's':
				case 'S':
					if(j+1 >= argc){
						libmin_printf("%s\n",USAGE);
						libmin_fail(1);
					}
					seed = libmin_atol(argv[j+1]);
					j++;
					continue;
				case 't':
				case 'T':
					if(j+1 >= argc){
						libmin_printf("%s\n",USAGE);
						libmin_fail(1);
					}
					trials = libmin_atoi(argv[j+1]);
					j++;
					continue;	
				case 'm':
				case 'M':
					if(j+1 >= argc){
						libmin_printf("%s\n",USAGE);
						libmin_fail(1);
					}
					max_fortune = libmin_atoi(argv[j+1]);
					j++;
					continue;
				case 'v':
				case 'V':
					libmin_printf("%s\n", VERSION);
					libmin_success();
				case '?':
				case 'h':
				case 'H':
					libmin_printf("%s\n",HELP);
					libmin_success();
				case '1':
					game_select = 0.0;
					break;
				case '2':
					game_select = 1.0;
					break;
				default:
					libmin_printf("parrondo: unkown option %s\n",
						argv[j]);
					libmin_fail(1);
			}
		else {
			libmin_printf("%s\n", USAGE);
			libmin_fail(1);
		}
	}
 
	/* If no seed is supplied, then use current system time */
	
	if(!seed)
  {
			seed = INITIAL_SEED; /* if all else fails */
			libmin_printf("Using seed = %d\n",INITIAL_SEED);
  }
		
	libmin_srand((int)seed);
	for(i=0;i<3;i++)site_visits[i] = 0L;  /* initialize counters */
	i=0;
	libmin_printf("Simulating %d trials ...\n",trials);
	while(i<trials){   /* Loop over trials */

		/* reseed */
		seed = libmin_rand();
		libmin_srand((int)seed);

		/* Each trial: loop until fortune goes out of range */
		fortune = 0;
		while(n++<MAX_ITERATIONS){
			if(cointoss(game_select) == 1)
				fortune += play_c(fortune);
			else
				fortune += play_s();
			if((fortune >= max_fortune)||(fortune <= -max_fortune))
				break;

			m = fortune > 0 ? fortune : -fortune;
			site_visits[m%3]++;	
		}

		if(fortune == max_fortune)
			win_count++;
		else if(fortune == -max_fortune)
			loss_count++;

		i++;
		n_tot += (double)n;
		n = 0L;
		
	}

	n_bar = n_tot/((double)i);

	/* Print stuff out */

	libmin_printf("%d wins, %d losses, %d draws\n",win_count,
			loss_count, i-(win_count+loss_count));
	libmin_printf("(Win/loss = %d/-%d, draw = no win/loss in %ld plays.)\n",
			max_fortune,max_fortune,MAX_ITERATIONS);
	libmin_printf("Average trial length = %lf\n",n_bar);
	libmin_printf("Site occupancy: 0 mod 3: %lf%%, 1 mod 3: %lf%%, 2 mod 3: %lf%%\n",
		100.0*((double)site_visits[0])/n_tot,
		100.0*((double)site_visits[1])/n_tot,
		100.0*((double)site_visits[2])/n_tot
	);

  libmin_success();
	return 0;
}

