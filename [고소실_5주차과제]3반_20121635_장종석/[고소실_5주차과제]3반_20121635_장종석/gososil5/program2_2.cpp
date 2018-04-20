#include "my_solver.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>

// global variables
const double DELTA = 0.0000000001;
const int Nmax = 100;
const double EPSILON = 0.00000000001;


double linear_interpolation(struct _DATA *F, int index, double x1){
	double temp;

	temp = ((F[index+1].y - F[index].y)/(F[index+1].x-F[index].x)) * (x1-F[index].x) + F[index].y;
	return temp;

}

double bisectin_method(struct _DATA *F, int N, double h, double U){
	double x0 = 0.0, x1 = 1.0;
	double a0 = 0.0, b0 = 1.0;
	double temp;
	int n, s, a;
	double t1, t2;
	
	x1 = (a0 + b0)/2.0;
	for( n = 0 ; ; n++ ){

		for( a= 0 ; a < N; a++){
			if( a*h <= a0 && a0 < (a+1)*h )
				break;
		}

		for( s= 0 ; s < N; s++){
			if( s*h <= x1 && x1 < (s+1)*h )
				break;
		}

		if( fabs( linear_interpolation(F, s, x1) -U) < DELTA || n > Nmax || fabs(x1-x0) < EPSILON)
			break;
		
		t1 = linear_interpolation(F, a, a0)-U;
		t2 = linear_interpolation(F, s, x1)-U;
		if( t1 * t2 < 0.0)
			b0 = x1;
		else
			a0 = x1;

		temp = (a0 + b0)/2.0;
		x0 = x1;
		x1 = temp;
		
	}
	return x1;
}

double calculate_Area(struct _DATA *A, int N,double h, double start, double end) {

	double area = 0.0;
	int s, e;

	for(e = 0; e < N; e++){
		if(e*h <= end && end < (e+1)*h) 
			break;
	}

	for(int i = 0; i <= e; i++) {
		if( e == 0 )
			break;

		if(i == 0 || i == e)
			area += A[i].y;
		else 
			area += A[i].y*2;
	}

	area = area * h / 2;
	return area;

}


void program2_2()
{
	
	struct _DATA *A, *F;
	FILE *fp_r, *fp_w;	
	int n_r, N;
	double U, h;
	int ii;
	double temp = 0.0;
	double x_start = 0.0;

	srand(time(NULL));

	fp_r = fopen("pdf_table.txt", "r");
	fp_w = fopen("random_event_table.txt", "w");

	scanf("%d", &n_r);

	if(fp_r == NULL) {
		printf("input file not found....\n");
		exit(0);
	}
	fscanf(fp_r, "%d %lf", &N, &h);
	A = (struct _DATA*)malloc(sizeof(struct _DATA)*N);
	F = (struct _DATA*)malloc(sizeof(struct _DATA)*N);
	
	for( ii = 0;ii < N; ii++){
		fscanf(fp_r, "%lf %lf", &A[ii].x, &A[ii].y);

		F[ii].x = A[ii].x;
		F[ii].y = calculate_Area(A, N,h, 0, F[ii].x);
	}
	
	fprintf(fp_w, "%d\n",n_r);
	while (n_r--) {
		U = (double)rand() / RAND_MAX; // [0, 1] ���̿� �����ϴ� ������ �� U

		x_start = bisectin_method(F, N, h, U);
		printf("[%.15lf]\n", x_start);

		fprintf(fp_w, "%.15lf\n",x_start);

		// Step 2: 4���� ������ �ۼ��ߴ� Bisection ����� ����� f(x*) = 0���� ����� �ִ� x* ���� ���Ѵ�. (����� DELTA, Nmax, EPSILON�� ���� ������� ���� ����)

		// �̶� Bisection�� �ʱ� ������ x�� [0, 1]�� ����ȭ �Ǿ����Ƿ� �ʱⰪ�� 0�� 1�� �д�. �̶��� ������ x*�̶�� ����.

		// �׷��� �츮�� ���Ϸ��� �ϴ� x*�� �̹� ��� ������ �� ���� sampling�� �����ϴ� x�� �ƴ� �ٸ� ���� ���� x*�� ã�� ���̴�.
		// ���� x*�� �ش��ϴ� F(x*)���� �𸣹Ƿ� f(x) = 0 �������� Ǯ�� ����ϴ�.
		// Step 3: ���� F(x*) ���� ��������(linear interpolation)�� �����Ͽ� �ٻ������� F(x*)�� ���Ѵ�.(�������� ������ �˻�, �ǽ��ð� ���� ����, 17�� 5.2 �κ� ����)
		
		// Step 4: �� ����� ���� ���� x*�� ��� ���˿� �°� random_event_table.txt�� ����Ѵ�.(15�� ����)
	}

	if(fp_r != NULL) fclose(fp_r);
	if(fp_w != NULL) fclose(fp_w);
}
