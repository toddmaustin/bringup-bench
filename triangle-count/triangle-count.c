#include "libmin.h"

#define SIZE 13
int64_t graph[SIZE][SIZE];
int64_t _graph[SIZE][SIZE] =
{
  {0,1,0,0,1,0,0,0,0,0,0,0,0},
	{1,0,1,1,0,0,0,0,0,0,0,0,0},
	{0,1,0,1,0,0,1,0,0,0,0,0,0},
	{0,1,1,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,1,0,1,0,0,0,0,0},
	{0,0,0,0,1,0,1,1,0,0,0,0,0},
	{0,0,1,0,0,1,0,0,0,1,1,0,0},
	{0,0,0,0,1,1,0,0,1,0,0,0,0},
	{0,0,0,0,0,0,0,1,0,0,0,0,0},
	{0,0,0,0,0,0,1,0,0,0,1,1,0},
	{0,0,0,0,0,0,1,0,0,1,0,0,1},
	{0,0,0,0,0,0,0,0,0,1,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,1,0,0}
};
	
int64_t squared[SIZE][SIZE];
int64_t cubed[SIZE][SIZE];

void
multiply(int64_t matrix1[][SIZE], int64_t matrix2[][SIZE], int64_t product[][SIZE])
{
	for(unsigned i = 0; i<SIZE; i++){
		for(unsigned j = 0; j<SIZE; j++){
			product[i][j] = 0;
			for(unsigned k = 0; k<SIZE; k++){
				product[i][j] = (int64_t)(product[i][j] + matrix1[i][k] * matrix2[k][j]);
			}
		}
	}
}

int64_t
trace(int64_t cubed[][SIZE])
{
	int64_t sum = 0;
	for(unsigned i = 0; i<SIZE; i++){
		sum = (int64_t)(sum + cubed[i][i]);
	}
	return sum;
}

int64_t
count(int64_t trace)
{
	return (trace / 6);
}

int
main(void)
{
  // initialize the pseudo-RNG
  libmin_srand(42);

  // initialize the encrypted inputs
  for (int i=0; i<SIZE; i++)
    for (int j=0; j<SIZE; j++)
      graph[i][j] = _graph[i][j];

	int64_t triangleCount;
	{
		// Stopwatch s("VIP_Bench Runtime");

		multiply(graph,graph,squared);
		multiply(squared,graph,cubed);
		int64_t diagonal = trace(cubed);
		triangleCount = count(diagonal);
	}
	libmin_printf("INFO: The number of triangles is: %lu\n", triangleCount);

  libmin_success();
  return 0;
}
