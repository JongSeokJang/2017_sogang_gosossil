#include "my_solver.h"
#include <stdlib.h>
#include <string.h>

void program3_4(void)
{
	
	float *A, *B, *X, *S;
	float *A_temp, *B_temp;
	float *confirm;
	int *L;
	int n, IA;
	int i, j;
	int info;
	FILE *fp_r, *fp_w;


	
	fp_r = fopen("linear_system_3-4.txt", "r");
	fp_w = fopen("solution_3-4.txt","w");

	
	fscanf(fp_r, "%d", &n);
	
	
	A = (float*)malloc(sizeof(float)*(n*n));	
	B = (float*)malloc(sizeof(float)*n);
	X = (float*)malloc(sizeof(float)*n);
	S = (float*)malloc(sizeof(float)*n);

	L = (int*)malloc(sizeof(int)*n);

	A_temp = (float*)malloc(sizeof(float)*(n*n));
	B_temp = (float*)malloc(sizeof(float)*n);

	
	for( i = 0 ; i < n ; i++){
		for( j = 0; j < n; j++){
			fscanf(fp_r, "%f", &A[j*n+i]);
			A_temp[j*n +i] = A[j*n+i];
		}
	}

	for( i = 0 ; i< n; i++){
		fscanf(fp_r, "%f", &B[i]);
		B_temp[i] = B[i];
	}

	memset(X, 0x00, sizeof(X));
	memset(S, 0x00, sizeof(S));
	memset(L, 0x00, sizeof(L));
	
	IA = n;
	
	info = gespp_(&n, A, &IA, L, S);
	info = solve_(&n, A, &IA, L, B, X);

	fprintf(fp_w, "%d\n", n);
	for(i = 0; i < n; i++)
		fprintf(fp_w, "%.6f\n", X[i]);
		

	confirm = (float*)malloc(sizeof(float)*n);
	memset(confirm, 0x00, sizeof(confirm));
	
	float sum = 0.0;
	for( i = 0; i < n ; i++){
		sum = 0.0;

		for ( j = 0 ; j < n ;j++){
			sum += A_temp[j*n+i] * X[j];
		}
		confirm[i] = sum - B_temp[i];
	}

	sum = 0.0;
	double temp = 0.0;

	for( i = 0 ; i < n; i++){
		sum += confirm[i]*confirm[i];
		temp += B_temp[i]*B_temp[i];
	}
	
	fprintf(fp_w, "\n%.6lf\n", sqrt(sum)/sqrt(temp));
		
	free(A);
	free(B);
	free(X);
	free(S);
	free(L);
	free(A_temp);
	free(B_temp);

	fclose(fp_r);
	fclose(fp_w);
	
}