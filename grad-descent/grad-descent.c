#include "libmin.h"

/** Gradient Descent for linear regression.
* The formula for linear regression is : Y= wX + b
* Cost Function: 1/n⅀(Yi - (wXi + b))^2 [i starts from 0 → n]
*Functions Description:
* 1) derivateWRTWeight(double weight, double bias):
* 	Is the partial derivative of the cost function with respect to weight
* 2) derivateWRTBias(double weight, double bias):
* 	Is the partial derivative of the cost function with respect to bias
* 3) gradientDescent(double weight, double bias):
*	Represents the gradient descent function. It is used to manage the value of the weight
*	and the bias of the linear function.
*/

#define M  50
double L = 0.001;

// double X[M] = {0, 1, 2, 3, 4, 5, 6, 7};
// double Y[M] = {1, 2, 3, 4, 5, 6, 7, 8};
//double X[M] = {-5, -4.7, -3, -2.5, -1, 1.5, 2, 2.3, 3, 3.9};
//double Y[M] = {3, 1.7, 2, 1.5, 4, -0.3, -1.5, -2, -4.4, -6};

//Number of homicides by firearm
double Y[M] = {
  56, 20, 1198, 9,30,18,11,52,1456,8,
  12,70,68,18,34678,0,51,187,173,353,
  12539,248,201,17,27,5,20,15,1618,1790,
  453,2446,41,3,24,35,24,158,29,5009,
  85,5201,7,0,3093,21,6,417,1080,11
}; //6

//Average firearms per 100 people
double X[M] = {
  8.6, 7.6,10.2,12.5,15,30.4,3.5,5.3,0.5,7.8,
  7.3,17.2,10,17.3,8,1.4,6.2,4.3,30.8,10.7,
  5.9,1.4,9.9,21.7,4.8,36.4,16.3,12,5.1,1.3,
  3.5,5.8,6.2,9.2,45.3,7.3,30.3,22.5,13.1,14.6,
  6.2,5.5,30.3,4.2,8.6,7.3,11.9,8.1,0.6,11.5
};


double
derivateWRTWeight(double weight, double bias)
{
	double sum = 0;
	
	for(int i = 0; i<M; i++){
		sum = (double)(sum + (X[i]*(Y[i] - (weight * X[i] + bias))));
	}
	
	return ((double)-2.0 * sum)/M;
}

double
derivateWRTBias(double weight, double bias)
{
	double sum = 0;
	
	for(int i = 0; i<M; i++){
		sum = (double)(sum + (Y[i] - (weight * X[i] + bias)));
	}
	
	return ((double)-2.0 * sum)/M;
}

void
gradientDescent(double *pweight, double *pbias)
{
	for (unsigned i = 0; i<200; i++)
  {
		*pweight = (double)(*pweight - (L*derivateWRTWeight(*pweight, *pbias)));
		*pbias =(double)(*pbias - (L*derivateWRTBias(*pweight, *pbias)));		
	}
}

int
main(void)
{
	double weight = 0;
	double bias = 0;

	gradientDescent(&weight, &bias);
	libmin_printf("The function is: %.4lfx + %.4lf\n", weight, bias);

  libmin_success();
  return 0;
}

