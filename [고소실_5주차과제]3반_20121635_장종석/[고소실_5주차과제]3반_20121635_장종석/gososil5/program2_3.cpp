#include "my_solver.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

const double DELTA = 0.0000000001;
const int Nmax = 100;
const double EPSILON = 0.00000000001;

#define DIVIDE_NUM 128
#define DIVIDE_SIZE 0.0078125

// HOMEWORK
void program2_3()
{
	
	FILE *fp_r, *fp_w = fopen("histogram.txt", "w");
	int i, data_size, Count[DIVIDE_NUM+1];
	double *X;

	fp_r = fopen("random_event_table_2_2_a.txt", "r");

	fscanf(fp_r, "%d", &data_size);
	X = (double *)malloc(sizeof(double)*data_size);
	
	memset(Count, 0x00, sizeof(Count));
	for (i = 0; i < data_size; i++) {
		fscanf(fp_r, "%lf", &X[i]);
		Count[(int)(X[i] / DIVIDE_SIZE)]++;
	}
	Count[DIVIDE_NUM-1] += Count[DIVIDE_NUM];
	fprintf(fp_w, "Histrogram of 2.2a\n");
	for (i = 0; i < DIVIDE_NUM; i++) {
		fprintf(fp_w, "%.7lf ~ %.7lf : %4d\t", i*DIVIDE_SIZE, i*DIVIDE_SIZE + DIVIDE_SIZE, Count[i]);
			
		for (int j = 0; j < Count[i]; j++) 
			fprintf(fp_w, "|");
		fprintf(fp_w, "\n");
	}
	free(X);

	if (fp_r != NULL) 
		fclose(fp_r);

	fp_r = fopen("random_event_table_2_2_b.txt", "r");

	fscanf(fp_r, "%d", &data_size);
	X = (double *)malloc(sizeof(double)*data_size);

	memset(Count, 0x00, sizeof(Count));
	for (i = 0; i < data_size; i++) {
		fscanf(fp_r, "%lf", &X[i]);
		Count[(int)(X[i] / DIVIDE_SIZE)]++;
	}
	Count[DIVIDE_NUM-1] += Count[DIVIDE_NUM];

	fprintf(fp_w, "\nHistrogram of 2.2b\n");
	for (i = 0; i < DIVIDE_NUM; i++) {
		fprintf(fp_w, "%.7lf ~ %.7lf : %4d\t", i*DIVIDE_SIZE, i*DIVIDE_SIZE + DIVIDE_SIZE, Count[i]);

		for (int j = 0; j < Count[i]; j++) 
			fprintf(fp_w, "|");
		fprintf(fp_w, "\n");
	}
	free(X);

	if (fp_r != NULL) fclose(fp_r);
	if (fp_w != NULL) fclose(fp_w);
	

}

// HOMEWORK
void program2_2_a()
{

	__int64 start, freq, end;
	float resultTime = 0;

	
	
	struct _DATA *A, *F;
	FILE *fp_r, *fp_w;
	int n_r, N;
	double U, h;
	int ii;
	double x_start = 0.0;

	srand(time(NULL));

	fp_r = fopen("pdf_table.txt", "r");
	fp_w = fopen("random_event_table_2_2_a.txt", "w");

	scanf("%d", &n_r);

	if (fp_r == NULL) {
		printf("input file not found....\n");
		exit(0);
	}
	fscanf(fp_r, "%d %lf", &N, &h);

	A = (struct _DATA*)malloc(sizeof(struct _DATA)*N);
	F = (struct _DATA*)malloc(sizeof(struct _DATA)*N);

	for (ii = 0; ii < N; ii++) {
		fscanf(fp_r, "%lf %lf", &A[ii].x, &A[ii].y);

		F[ii].x = A[ii].x;
		F[ii].y = calculate_Area(A, N, h, 0, F[ii].x);
	}

	CHECK_TIME_START;

	fprintf(fp_w, "%d\n", n_r);
	srand((unsigned int)time(NULL));
	while (n_r--) {

		U = (double)rand() / RAND_MAX; // [0, 1] 사이에 존재하는 임의의 값 U

		x_start = bisectin_method(F, N, h, U);
		//printf("[%.15lf]\n", x_start);

		fprintf(fp_w, "%.15lf\n", x_start);

	}

	CHECK_TIME_END(resultTime);

	if (fp_r != NULL) fclose(fp_r);
	if (fp_w != NULL) fclose(fp_w);

	
	

	printf("The program2_2_a run time is %f(ms)..\n", resultTime*1000.0);
}

void program2_2_b()
{
	__int64 start, freq, end;
	float resultTime = 0;

	
	
	struct _DATA *A, *F;
	FILE *fp_r, *fp_w;
	int n_r, N;
	double U, h;
	int ii;
	double x_start = 0.0;

	srand(time(NULL));

	fp_r = fopen("pdf_table.txt", "r");
	fp_w = fopen("random_event_table_2_2_b.txt", "w");

	scanf("%d", &n_r);

	if (fp_r == NULL) {
		printf("input file not found....\n");
		exit(0);
	}
	fscanf(fp_r, "%d %lf", &N, &h);

	A = (struct _DATA*)malloc(sizeof(struct _DATA)*N);
	F = (struct _DATA*)malloc(sizeof(struct _DATA)*N);

	for (ii = 0; ii < N; ii++) {
		fscanf(fp_r, "%lf %lf", &A[ii].x, &A[ii].y);

		F[ii].x = A[ii].x;
		F[ii].y = calculate_Area(A, N, h, 0, F[ii].x);
	}

	CHECK_TIME_START;

	fprintf(fp_w, "%d\n", n_r);
	srand((unsigned int)time(NULL));
	while (n_r--) {
		U = (double)rand() / RAND_MAX; // [0, 1] 사이에 존재하는 임의의 값 U
		x_start = secant_method(F, N, h, U);
		//printf( "[%.15lf]\n", x_start);
		fprintf(fp_w, "%.15lf\n", x_start);
	}

	CHECK_TIME_END(resultTime);

	if (fp_r != NULL) fclose(fp_r);
	if (fp_w != NULL) fclose(fp_w);

	
	// something to do...

	

	printf("The program2_2_b run time is %f(ms)..\n", resultTime*1000.0);
}



double secant_method(struct _DATA *F, int N, double h, double U)
{
	double x0 = 0.0, x1 = 1.0;
	double a0 = 0.0, b0 = 1.0;
	double temp;
	int n, s, a;
	double t1, t2;

	// do bisection
	x1 = (a0 + b0)/2.0;
	
	for( n = 0 ; ; n++ ){

		for( a= 0 ; a < N; a++){
			if( a*h <= a0 && a0 < (a+1)*h )
				break;	
		} // a is left

		for( s= 0 ; s < N; s++){
			if( s*h <= x1 && x1 < (s+1)*h )
				break;
		} // s is x1

		if( fabs( F[s].y -U ) < DELTA || n > Nmax || fabs(x1-x0) < EPSILON )
			break;

		t1 = F[a].y - U;
		t2 = F[s].y - U;

		if( t1 * t2 < 0.0)
			b0 = x1;
		else
			a0 = x1;

		temp = (a0 + b0)/2.0;
		x0 = x1;
		x1 = temp;
	}
	

	for (n = 0; ; n++) {

		for (a = 0; a < N; a++) {
			if (a*h <= x0 && x0 < (a + 1)*h)
				break;
		} // a is x0

		for (s = 0; s < N; s++) {
			if (s*h <= x1 && x1 < (s + 1)*h)
				break;
		} // s is x1

		if (fabs(F[s].y - U) < DELTA || n > Nmax || fabs(x1 - x0) < EPSILON)
			break;

		temp = x1 - F[s].y*((s-a) / (F[s].y - F[a].y));
		x0 = x1;
		x1 = temp;
	}
	
	return x1;
}
