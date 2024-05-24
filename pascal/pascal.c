/* pascal.c: print rows of Pascal's triangle to stdout. The algorithm
             is simply to generate successive rows iteratively using the
             defining property of pascal's triangle:

			C(n,k) = C(n-1,k-1) + C(n-1,k).

	     The program is written for simplicity rather than efficiency.

	     The only tricky thing is getting the display to look right.
             We provide an option -c to compute the number of display
             columns needed without doing the display itself. 

	     Author: Terry R. McConnell

	     Compile: cc -o pascal pascal.c
*/

#include "libmin.h"

#define PROGRAM_NAME "pascal"
#define VERSION "1.0"
#define MAX_ROWS 30
#define USAGE "usage: pascal [ -h -v] [-c rows] rows\n"
#define HELP "\n\npascal [-h -v] [-c n] n \n\
print the first n rows of pascal's triangle to stdout.\n\
-h: print this helpful information\n\
-v: print version number and exit\n\
-c: print the number of columns required for display only \n\n"

static int triangle[MAX_ROWS + 1][MAX_ROWS];

/* num_digits: count how man digits the argument has and return that number */

int num_digits(int n) {

	int d = 1;

	
	while((n=n/10) != 0)
    d++;
	return d;
}

/* print_centered: print the first argument centered in a field of 
	width given by the second argument, to the extent possible.
	(The width is always the second argument, but the number may not
        be quite centered, depending on parity.) 
	Return 0 if successful and 1 if not.
*/

int print_centered(int n, int width)
{
	int nw;
	int pad_space;
	int i;

	nw = num_digits(n);
	if(width < nw) return 1;  /* no can do */

	pad_space = (width - nw)/2;

	for(i=0;i<pad_space;i++)
	  libmin_printf(" ");
	libmin_printf("%d",n);
	for(i=0;i<width - nw - pad_space;i++)
	  libmin_printf(" ");
	return 0;
}

int main(void)
{
	int argc=1; char **argv=NULL;
	int i=1,j;
	int cols_option = FALSE;
	int nrows;
	int max_width;
	int row_offset;

	/* Process command line */
	
	while(((i+1)<=argc)&&(argv[i][0] == '-')){
		if(argv[i][1] == 'h'){
			libmin_printf(HELP);
			return 0;
		}
		if(argv[i][1] == 'v'){
			libmin_printf("%s\n",VERSION);
			return 0;
		}
		if(argv[i][1]=='c'){
			/* make sure there is an arg */
			if((i+1) >= argc){
				libmin_printf(USAGE);
				return 1;
			}
			cols_option = TRUE;
			i++;
			continue;
		}
		/* Unknown option if we got to here */
		libmin_printf(USAGE);
		return 1;
	}

	/* Make sure there is one arg left */

	if((i+1) != argc){ 
	  nrows = 20;
	}
	else
	  nrows = libmin_atoi(argv[i]);

	/* Sanity checks */
	if(nrows == 0){ 
		if(cols_option)
		  libmin_printf("0\n");
		libmin_success(); /* nothing to do */
	}
	if(nrows < 0){
		libmin_printf("%s: %d is an invalid number of rows\n", PROGRAM_NAME,nrows);
		libmin_fail(1);
	}
	if(nrows > MAX_ROWS){
		libmin_printf("%s: %d > %d rows maximum\n",PROGRAM_NAME,nrows, MAX_ROWS);
		libmin_fail(1);
	}

	/* build the triangle */
	
	triangle[0][0] = 1;
	for(i=1;i<nrows;i++){
		triangle[i][0] = 1;
		for(j=1;j<i;j++)
			triangle[i][j] = triangle[i-1][j-1]
				+ triangle[i-1][j];
		triangle[i][i] = 1;
	}

	/* Find the widest number in table. It's the middle one in
           the bottom row */

	max_width = num_digits(triangle[nrows-1][nrows/2]);

	/* make sure this value is even */
	if(max_width % 2) max_width++;

	/* Since each number is printed in a field max_width+2 wide
	   and there are nrows numbers in the longest (bottom) row
           the number of columns needed for the display is
           (max_width+2)*nrows */

	if(cols_option){
			libmin_printf("%d\n",(max_width + 2)*nrows);
			libmin_success();
	}
	   
	/* Now print everything out */ 

	for(i=0;i<nrows;i++){

		/* This is the only tricky part. Think of each number as
                   being printed in the center of a brick. Each brick has
                   width max_width/2 + 1. Going down one row, the bricks
                   move over one half width */

		row_offset = (nrows-i-1)*(max_width/2+1);

		/* Move first row in deepest, etc., by printing the approp.
                   number of leading blanks */

		for(j=0;j<row_offset;j++)
		  libmin_printf(" ");

		/* Print ith row of numbers */

		for(j=0;j<i+1;j++)
			if( print_centered(triangle[i][j],max_width+2)){
				libmin_printf("%s: error printing element %d,%d\n",PROGRAM_NAME,i,j);
				libmin_fail(1);
		}
		libmin_printf("\n");
	}

  libmin_success();
	return 0;
}

