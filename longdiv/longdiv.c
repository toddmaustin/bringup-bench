/* longdiv.c: program to print out examples of "long-division" problems.
*  (Sure wish I had one of these in grade school.)
*
* By Terry R. McConnell 12/97
*
* Usage: longdiv <dividend> <divisor>
* Here both dividend and divisor must be positive integers. At some point
* we should improve the program to handle decimal points, negative
* numbers, and other bases,
* but this should be sufficient to establish the concept.
*
* Dividend and divisor can be arbitrarily long, subject only to the size of
* ARG_MAX on your system. POSIX.1 ensures that this is at least 4096 bytes.
* I.e, this can handle REALLY LARGE division problems!
*
* A bit of useful terminology: 
*   dividend = that which is divided into ( = numerator ).
*   divisor  = that which is divided by ( = denominator ).
*   subtrahend = that which is subtracted.
*   minuend  = that which is subtracted from.
*/


#define _POSIX_SOURCE
#define USAGE "Usage: longdiv <dividend> <divisor>"
#define DIGITS "123456789"

#include "libmin.h"

#define RADIX 10 /* Actually, base 10 is assumed at many points in the pgm,
                    so porting this to handle other bases will involve more
                    than just changing this define. */

#define BORROW 1
#define NO_BORROW 0


/* The main routine is at the bottom, following a number of auxiliary
   routines.
*/

/* Subtracts digits and stores result in res. Returns indicator of whether
   or not borrow is needed.
*/

int
sub( char x, char y, char *res)
{
	if( x >= y){
		 *res = '0' + x - y;
		 return FALSE;
	}
	*res = '0' + x - y + 10;
	return TRUE;
}

/* Isgte: returns TRUE if first digit string is >=  the second */

int
isgte( char *A, char *B)
{
	int a,b,i;
	char *AA, *BB;

	if(B==NULL) return TRUE;
	if(A==NULL) return FALSE;

/* Normalize by stripping off leading zeros */

	AA = libmin_strpbrk(A,DIGITS);
	BB = libmin_strpbrk(B,DIGITS);

	if(BB == NULL) return TRUE;
	if(AA == NULL) return FALSE;  

	if((b=libmin_strlen(BB ))>(a=libmin_strlen(AA)))return FALSE;
	if(a > b) return TRUE;
	
	for(i=0;i<b;i++){
		if(AA[i]>BB[i]) return TRUE;
		if(AA[i]<BB[i]) return FALSE;
	}
	return TRUE;
}	
		 
	

/* Given character strings of digits with mend >= shend, this returns a pointer
to the string corresponding to minuend - subtrahend. Returns NULL if
minuend < subtrahend. Result array is padded with leading zeros.  
*/

char *sbc(char *mend, char *shend)
{
	int need_borrow = FALSE;
	int lm,ls,i;
	char *res;
	char *mnd,*p;

	if(!isgte(mend,shend))return NULL;

	lm = libmin_strlen(mend);
	ls = libmin_strlen(shend);
	res = (char *)libmin_malloc(libmin_strlen(mend)+1);
	if(res == NULL){
		libmin_printf("sbc: Unable to malloc space for result\n");
		libmin_fail(1);
	}
	p = mnd = (char *)libmin_malloc(libmin_strlen(mend)+1);
	if(mnd == NULL){
		libmin_printf("sbc: Unable to malloc\n");
		libmin_fail(1);
	}
	libmin_strcpy(mnd,mend);

	/* Fill result array with digit zero */

	for(i=0;i<lm;i++)res[i]='0';
	res[lm] = '\0';


	for(i = 0; i<lm; i++){
		if (need_borrow)
    {
			if(mnd[lm-i-1] == '0')
				mnd[lm-i-1] = '9'; /* need_borrow stays TRUE */
			else {
				--mnd[lm-i-1];
				need_borrow = FALSE;
			}
    }
		if( i < ls )
		 need_borrow |= sub(mnd[lm-i-1],shend[ls-i-1],res+lm-i-1);
		else
		 need_borrow |= sub(mnd[lm-i-1],'0',res+lm-i-1);
	}
		
	libmin_free(p);
	return res;
}

/* Multiplies digit string by digit character. Returns string pointing
   to answer. Leading zeros are removed.
*/

char *
times_digit(char *mcand, char dgt)
{

	int i,len;
	int carry=0,idgt,curdgt,prod;
	char *res,*tmp;

	if(mcand == NULL) return NULL;

	idgt = dgt - '0';
	len = libmin_strlen(mcand);
	tmp = (char *)libmin_malloc(len+2);
	if(tmp==NULL){
		libmin_printf("Unable to malloc partial product array\n");
		libmin_fail(1);
	}
	
	tmp[len+1]='\0';

	for(i=0;i<len;i++){
		curdgt = mcand[len-i-1] - '0';
		prod = curdgt*idgt + carry;
		tmp[len-i] = '0' + (prod % RADIX);	
		carry = prod/RADIX;
	}
	tmp[0] = '0' + carry;
	if(tmp[0]=='0'){
		res = (char *)libmin_malloc(len+1);
		if(res == NULL){
			libmin_printf("Unable to malloc partial product array\n");
			libmin_fail(1);
		}
		res[len]='\0';
		libmin_strcpy(res,tmp+1);
		libmin_free(tmp);
	}
	else res = tmp;
	return res;
}

/* Allocates a new string consisting of front and back glued together */

char *
splice(char *front, char *back)
{

	char *res;
	int len;

	if(front == NULL)return back;
	if(back == NULL) return front;  
	len = libmin_strlen(front)+libmin_strlen(back);

	res = (char *)libmin_malloc((len+1)*sizeof(char));
	if(res == NULL) {
		libmin_printf("splice: unable to malloc\n");
		return NULL;
	}
	libmin_strcpy(res,front);
	libmin_strcat(res,back);
	return res;
}

/* Creates a new string by cutting the given number of characters off
   the front of source */

char *
cut_off(char *source, int n)
{
	int k;
	char *res;

	if((n <= 0)||(source == NULL))return NULL;
	k = libmin_strlen(source);
	k = ( n > k ? k : n );
	res = (char *)libmin_calloc(sizeof(char),(k+1));
	if(res == NULL){
		libmin_printf("cut_off: Unable to malloc\n");
		return NULL;
	}
	libmin_strncpy(res,source,k);
	return res;
}
	
int argc = 3;
char *argv[] = { "longdiv", "52333498", "27" };
int 
main(void)
{
	int dsr_len,ddnd_len;
	int dgts_fwd;   /* digits carried forward from previous step */
	int i,j,k;
	int step = 0;
	char *quotient;
	char *dsr;             /* Divisor */
	char *ddnds[128];   /* Successive dividends */
	char *shends[128]; /* Successive subtrahends */
	char *mend;  /* Current minuend: see example below */
	char *pend;  /* Points to 1st char beyond current minuend */
	char *ptr;

	/* Do sanity checks on args */

	for(i=0; i<libmin_strlen(argv[1]); i++)
		if(!isdigit(argv[1][i])){
			libmin_printf("%s\n%s\n","longdiv: syntax error",
				argv[1]);
			for(j=0;j<i;j++)libmin_putc(' ');
			libmin_putc('^');
			libmin_putc('\n');
			return FALSE;
		}

	for(i=0; i<libmin_strlen(argv[2]); i++)
		if(!isdigit(argv[2][i])){
			libmin_printf("%s\n%s\n","longdiv: syntax error",
				argv[2]);
			for(j=0;j<i;j++)libmin_putc(' ');
			libmin_putc('^');
			libmin_putc('\n');
			return FALSE;
		}

	/* Make sure we're not dividing by 0 */

	j = TRUE; /* Guilty, till proven innocent */
	for(i=0;i<libmin_strlen(argv[2]);i++)
		if(argv[2][i] != '0'){j = FALSE; break; }
	if(j){
		libmin_printf("%s\n", "longdiv: Cannot divide by 0.\n");
		return FALSE;
	}

	/* OK, lookin' good */
	/* Save divisor and dividend */

	if((ptr = libmin_strpbrk(argv[1],DIGITS))==NULL)
		 ddnd_len = 1;
	else 
	   ddnd_len = libmin_strlen(ptr);
	ddnds[0] = (char *)libmin_malloc((ddnd_len+1)*sizeof(char));
	if(ptr == NULL)
		libmin_strcpy(ddnds[0],"0");
	else
	        libmin_strcpy(ddnds[0],ptr);
	ptr = libmin_strpbrk(argv[2],DIGITS);
	dsr_len = libmin_strlen(ptr);
	dsr = (char *)libmin_malloc((dsr_len+1)*sizeof(char));
	libmin_strcpy(dsr,ptr);

	dgts_fwd = dsr_len-1; /* Turns out to be the right initialization */

	/* Reserve space for, and properly terminate, quotient */

	quotient = (char *)libmin_calloc(sizeof(char),ddnd_len+1);

	/* stick an appropriate number of leading zeros on quotient */
	/* These will be stripped in the printout */

	for(j=0;j<dsr_len-1;j++)quotient[j]='0';

	/* The algorithm breaks into steps, each of which involves a
	   set of trial multiplications and a subtraction. 
	   As a try at the first minuend, we take the first n digits of
	   the first dividend, where n is length of the divisor. To
	   clarify the terminology, consider the problem:

		9
          _______________________
   3213   ) 29946712
	    28917
	    -------
	       29
	
	Here, the first minuend is 29546, the first subtrahend is
	28917, and dgts_fwd is 2.  This was step = 0.  
        The next dividend will be 29712, and the next minuend will be gotten
        by taking the appropriate initial string from it. 
	*/
	
        while(isgte(ddnds[step],dsr)){  /* loop until dividend shorter than
                                             divisor. When we exit the loop,
                                             the last dividend is the remainder
					*/

	  /* Determine the next minuend */
	  mend = cut_off(ddnds[step],++dgts_fwd);	
	  while(!isgte(mend,dsr)){
		libmin_strcat(quotient,"0");
		libmin_free(mend);
		mend = cut_off(ddnds[step],++dgts_fwd);
	  } 

	  pend = ddnds[step]+libmin_strlen(mend); /* Set pointer to rest of dividend that
					 will be spliced on to form the
                                         next dividend. (points to 7 in example
					 above. */

	/* Now we do some "trial multiplications" to determine the next
           digit of the quotient. */

	  for(i='9';i>='1';i--){
		shends[step] = times_digit(dsr,i);
		if(isgte(mend,shends[step]))break;
		libmin_free(shends[step]);
	  }

	/* 
	  Insert the new digit in the quotient.  
	*/

	  quotient[libmin_strlen(quotient)]=i;

	/* Now, subtract the current subtrahend from the current minuend,
	   and splice the result with pend to form the next dividend */

	  ptr = sbc(mend,shends[step]);
	  if(libmin_strpbrk(ptr,DIGITS)==NULL)dgts_fwd = 0;
	  else
	  	dgts_fwd = libmin_strlen(libmin_strpbrk(ptr,DIGITS));

	/* A special situation arises here if dgts_fwd = 0 and pend
           points to a zero: since we strip off leading zeros when defining
           the new dividend, we would miss the need to append zero digits
           to the quotient.
	*/
	 if(dgts_fwd == 0)
		while ((*pend == '0') && (*pend != '\0')){
			libmin_strcat(quotient,"0");
			pend++;
		} 
	  ddnds[step+1] = libmin_strpbrk(splice(ptr,pend),
				DIGITS); 
	  if(ddnds[step+1]==NULL){
		ddnds[step+1]= libmin_malloc(2*sizeof(char));
		libmin_strcpy(ddnds[step+1],"0");
	  }
	  libmin_free(mend);
	  step++;

	}  /* repeat with new dividend */

	/* Add any necessary trailing zeros to quotient */
	j = libmin_strlen(quotient);
	for(i=0;i<ddnd_len - j;i++)libmin_strcat(quotient,"0");

	/* Calculation done. Print everything out */

	/* Print out the quotient */
	for(i=0;i<dsr_len+2;i++)libmin_putc(' ');
	for(;*quotient == '0';quotient++)libmin_putc(' ');
	libmin_printf("%s\n",quotient);

	/* Print out the top bar */
	
	for(i=0;i<dsr_len+2;i++)libmin_putc(' ');   /* space over */
	for(i=0;i<ddnd_len;i++)libmin_putc('_');

	/* Print out divisor and dividend */
	libmin_printf("\n %s)%s\n",dsr,ddnds[0]);

	for(i=0;i<step;i++){
		for(j=0;j<dsr_len+2+ddnd_len - libmin_strlen(ddnds[i]);j++)
			libmin_putc(' ');	
		libmin_printf("%s\n",shends[i]);
		for(j=0;j<dsr_len+2+ddnd_len - libmin_strlen(ddnds[i]);j++)
			libmin_putc(' ');	
		for(k=0;k<libmin_strlen(shends[i]);k++)libmin_putc('-');
		libmin_putc('\n');
		for(j=0;j<dsr_len+2+ddnd_len - libmin_strlen(ddnds[i+1]);j++)
			libmin_putc(' ');	
		libmin_printf("%s\n",ddnds[i+1]);
	}	
		
  libmin_success();
	return 0;
}	

