/* Ackermann.c: calculate the Ackermann function (as far as that is possible),
 * and discuss its properties. 

   Compile: cc -o ackermann ackermann.c

   ( Use the flag -D_SHORT_STRINGS if your compiler cannot handle multiline
     strings.)

   For usage information, give the command ackermann -h or see USAGE defined
   below.

   Author: Terry R. McConnell
	   trmcconn@syr.edu


	   Ackermann's function a(x,y) is defined by the following double
recursion:
	
	a(x,0) = x + 1
	a(0,y) = a(1,y-1)
	a(x+1,y+1) = a(a(x,y+1),y). 

The significance of this function is that it can be proved to be a recursive
function of two variables. Thus, in particular, a(x,x) is recursive. But it
can also be shown that a(x,x) is not primitive recursive. (The difference
between recursive and primitive recursive is that computation of a recursive
function may, in general, involve a finite but unbounded search for a solution
to an equation involving recursive functions. In the case of primitive
recursive functions there must be an a priori primitive recursive upper bound
on the number of steps in the search.)

Ackermann's function grows so quickly, especially in the y variable, that it
is completely infeasible to calculate it for more than a few small values of
y. For example, things really begin to blow up in the calculation of a(1,4). 

At the outset, it is not even clear that the above equations define a function,
i.e., that the recursions will ever end. Let us begin by proving by induction
that this will indeed occur. We induct on y, and then on x for each fixed y.
(This is typical of arguments involving a.) The case y = 0 is obvious, so
we assume the following outer inductive hypothesis:

(i) The recursive depth calculating a(x,y) is finite, for every x.

(By recursive depth, we mean the total number of times "a" must be written
in successive applications of the 3 rules until a call with y = 0 occurs.)

We wish to show: the recursive depth calculating a(x,y+1) is finite for every 
x. This we do by induction on x. If x = 0, then a(x,y+1) = a(1,y), and the
result follows by inductive hypothesis (i). Let us now make inductive
hypothesis (ii):

(ii) The recursive depth calculating a(x,y+1) is finite, for a given x.

There remains to show: the recursive depth calculating a(x+1,y+1) is finite.
Now a(x+1,y+1) = a(a(x,y+1),y). By (ii), a(x,y+1) is computable. Call its value
N. Then a(N,y) is computable by (i), since (i) holds for every x, in particular
for x = N.

Exercise 3.33, beginning on page 146 of Elliott Mendelson, An Introduction
to Mathematical Logic, 3rd Edition, Wadworth & Brooks/Cole, Monterey CA, 1897,
attempts to lead the reader through proofs of the main results about a(x,y).
Unfortunately, I cannot make sense out of the first several steps, though
the upshot, in part (j), seems reasonable. Part (k - IV), showing that
a is recursive, seems impossibly difficult given the tools available. Better
to wait and do exercise 5.40 after having studied Turing machines. The
last two parts showing that a(x,x)+1 is not primitive recursive are doable
and instructive using what has gone before.

Let us conclude this discussion by giving an idea of how one proves a(x,x)
is not primitive recursive. For simplicity, we deal only with functions of
one variable. The idea is to show that for any fixed primitive recursive
function f there is an integer m = m(f) such that f(x) <= a(x,m). To show
this, it suffices to prove it directly for the base functions (successor,
etc,) and to show that this property is preserved under substitution and
recursion. For example, suppose f is defined by the following particularly
simple type of recursion:

	f(0) = 0, f(y+1) = h(f(y)), 

for some primitive recursive h. We suppose m is such that h(x) < a(x,m) for
all x and show that f inherits this property with a different m. Indeed, take
M = m+1. Show by induction on y that f(y) <= a(y,M). This is obvious for
y = 0. Thus it suffices to show that f(y) <= a(y,M) -> f(y+1) <= a(y+1,M). 

Lemma (i) a(x,y) > x
      (ii) a(x,y) is increasing in x for each y.

Assuming this, we have f(y+1) = h(f(y)) <= a(f(y),m) <= a(a(y,M),m) (by (ii)
and inductive hypothesis) = a(a(y,M),M-1) = a(y+1,M). 

We prove (i) by induction on y. For y = 0 it reduces to x+1 > x. Thus suppose
a(x,y) > x for all x. To show a(x,y+1) > x. First note that a(0,y+1) =
a(1,y) > 1 > 0 by inductive hypothesis. Now, if x > 1, a(x,y+1) =
a(a(x-1,y+1),y) > a(x-1,y+1), again by inductive hypothesis. Iterating this,
a(x,y+1) > a(x-1,y+1) > a(x-2,y+1) > ... > a(0,y+1) > 1. The result follows
since there are x inequalities and each one is strict.

For (ii), we prove a(x,y) < a(x+1,y) by induction on y. For y = 0 it
reduces to x+1 < x+2. Suppose a(x,y) < a(x+1,y). We must show a(x,y+1) <
a(x+1,y+1). But a(x+1,y+1) = a(a(x,y+1),y) > a(x,y+1) by (i).

Finally, suppose a(x,x)+1 were primitive recursive. Then for some m we 
would have a(x,x) + 1 < a(x,m). But this is contradiction when x = m.

*/


#include "libmin.h"

#define AMAX 5  /* largest arguments to contemplate. This is waaaaaaaaaay more than sufficient! */

/* Play around with these definitions. Can you get a(1,4)? */
#define MAX_X 0xFFFF  /* rows in storage array */
#define MAX_Y 0x10    /* columns in storage array */
#define MAX_DEPTH 0xffFFFF /* stack guard */

static unsigned a[MAX_X][MAX_Y]; /* Remembered values */
static unsigned depth;
static unsigned max_depth;

/* Implement Ackermann function as recursive function that remembers its values */
unsigned
ack(unsigned x, unsigned y)
{
	depth++;
	if (depth > MAX_DEPTH)
  {
		libmin_printf("Maximum stack depth %d exceeded. Abort.\n", MAX_DEPTH);
		libmin_fail(1);
	}
	if (x >= MAX_X)
  {
		libmin_printf("Maximum x value %d exceeded. Abort. \n", MAX_X);
		libmin_fail(1);
	}
	if (y >= MAX_Y)
  {
		libmin_printf("Maximum y value %d exceeded. Abort. \n", MAX_Y);
		libmin_fail(1);
	}
	if (a[x][y])
    return a[x][y];
	if (y==0)
    return a[x][0] = x+1;
	if (x==0)
    return a[0][y] = ack(1,y-1);
  return a[x][y] = ack(ack(x-1,y),y-1);
}	

int
main(void)
{
	unsigned y,k; 

  max_depth = 0;
	for(k=0;k<=AMAX;k++)
  {
		libmin_printf("\nx+y=%d:\n\n",k);
		for(y=0;y<=k;y++)
    {
		  depth = 0;  /* stack guard */
			libmin_printf("A(%d,%d) = %d\n",k-y,y,ack(k-y,y));
      if (depth > max_depth)
        max_depth = depth;   
		}
	}
  libmin_printf("Max recursive depth = %u\n", max_depth);

  libmin_success();
  return 0;
}

