#include "libmin.h"

// Developer- Dik T. Winter
// Intro- Calculates the first 15000 digits of PI.
// Details- http://stackoverflow.com/questions/20287513/can-anyone-make-heads-or-tales-of-this-spigot-algorithm-code-pitiny-c
// Source- http://www.iwriteiam.nl/SigProgC.html#pi
 
int a[52514],b,c=52514,d,e,f=1e4,g,h;int main(void){libtarg_start_perf();for(;(b=c-=14);h=libmin_printf("%04d",e+d/f))for(e=d%=f;(g=--b*2);d/=g)d=d*b+f*(h?a[b]:f/5),a[b]=d%--g;libtarg_stop_perf();libmin_success(); return 0;}

