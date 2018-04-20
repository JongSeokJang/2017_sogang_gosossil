#include <stdio.h>
#include <random>
#include <time.h>
#include <math.h>
#include <time.h>
#include <Windows.h>

#define DEGREE 10
#define delta 0.0000001
#define N_X 1048576
void Eval_Poly_Naive(double y[], double x[], int n_x, double a[], int deg);
void Eval_Poly_Horner(double y[], double x[], int n_x, double a[], int deg);
__int64 start, freq, end;

#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f))
float compute_time;
void main(void)
{
	/*
		실습 2번의 경우, 다항식을 효율적으로 계산하는 방법에 관한 실습이었는데,
		실습 2-1의 경우 내장함수인 pow()를 사용하여 식을 만드는 방법,
		실습 2-2의 경우, Horner's rule 를 사용하는 것이었다. 두 케이스를 비교하면
		Horner's rule를 사용하는 방식이 내장한수 pow() 를 사용하여 계산하는 것보다
		매우 빠른 것을 알 수 있었다.

	*/


	int i;
	
	double a[DEGREE+1];
	double *x = (double *)malloc(sizeof(double)*N_X);
	double *y_n = (double *)malloc(sizeof(double)*N_X);
	double *y_h = (double *)malloc(sizeof(double)*N_X);

	for(i = 0; i < N_X; i++)
		x[i] = (double) rand()/((double) RAND_MAX);

	for(i = 0; i <= DEGREE; i++)
		a[i] = (double) rand();

	CHECK_TIME_START;
	Eval_Poly_Naive(y_n, x, N_X, a, DEGREE);
	CHECK_TIME_END(compute_time);
	printf("Eval_Poly_Naive: %f\n", compute_time);


	CHECK_TIME_START;
	Eval_Poly_Horner(y_h, x, N_X, a, DEGREE);
	CHECK_TIME_END(compute_time);
	printf("Eval_Poly_Horner: %f\n", compute_time);

	for(i = 0; i < N_X; i++){
		//printf("%f %f %.8f", y_n[i], y_h[i], y_n[i]-y_h[i]);
		if(fabs(y_n[i] - y_h[i]) > delta) 
			break;
	}
	
	if(i == N_X) 
		printf("Calculated normally.\n");
	else 
		printf("Calculated abnormally.\n");
		

	free(x);
	free(y_h);
	free(y_n);

}
void Eval_Poly_Naive(double y[], double x[], int n_x, double a[], int deg)
{
	int i, j;
	for(i = 0; i < n_x; i++){

		for(j = 0, y[i] = 0.0; j <= deg; j++){
			y[i] += a[j] * pow(x[i], j);
		}
	}
}
void Eval_Poly_Horner(double y[], double x[], int n_x, double a[], int deg)
{
	int i, j;
	for(i = 0; i < n_x; i++){
		for(j = deg-1, y[i] = a[deg]; j >= 0; j--){
			y[i] = y[i]*x[i] + a[j];
		}
	}
	
}
