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
		U = (double)rand() / RAND_MAX; // [0, 1] 사이에 존재하는 임의의 값 U

		x_start = bisectin_method(F, N, h, U);
		printf("[%.15lf]\n", x_start);

		fprintf(fp_w, "%.15lf\n",x_start);

		// Step 2: 4주차 숙제로 작성했던 Bisection 방법을 사용해 f(x*) = 0으로 만들어 주는 x* 값을 구한다. (상수인 DELTA, Nmax, EPSILON은 본인 마음대로 조정 가능)

		// 이때 Bisection의 초기 구간은 x가 [0, 1]로 정규화 되었으므로 초기값을 0과 1로 둔다. 이때의 중점을 x*이라고 하자.

		// 그러나 우리가 구하려고 하는 x*은 이미 곡선을 설계할 때 사용된 sampling에 존재하는 x가 아닌 다른 유사 난수 x*를 찾는 것이다.
		// 따라서 x*에 해당하는 F(x*)값을 모르므로 f(x) = 0 방정식을 풀기 곤란하다.
		// Step 3: 따라서 F(x*) 값은 선형보간(linear interpolation)을 적용하여 근사적으로 F(x*)을 구한다.(선형보간 내용은 검색, 실습시간 설명 내용, 17쪽 5.2 부분 참고)
		
		// Step 4: 위 방법을 통해 구한 x*을 출력 포맷에 맞게 random_event_table.txt에 출력한다.(15쪽 참고)
	}

	if(fp_r != NULL) fclose(fp_r);
	if(fp_w != NULL) fclose(fp_w);
}
