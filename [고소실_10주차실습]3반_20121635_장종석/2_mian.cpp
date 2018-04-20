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
		�ǽ� 2���� ���, ���׽��� ȿ�������� ����ϴ� ����� ���� �ǽ��̾��µ�,
		�ǽ� 2-1�� ��� �����Լ��� pow()�� ����Ͽ� ���� ����� ���,
		�ǽ� 2-2�� ���, Horner's rule �� ����ϴ� ���̾���. �� ���̽��� ���ϸ�
		Horner's rule�� ����ϴ� ����� �����Ѽ� pow() �� ����Ͽ� ����ϴ� �ͺ���
		�ſ� ���� ���� �� �� �־���.

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
