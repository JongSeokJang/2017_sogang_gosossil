#include "my_solver.h"

//printArea(A, N, 0.0, 1.0);
void printArea(struct _DATA *A, int N, double start, double end) {

	double area = 0.0;
	double h = (double)1/(N-1);
	int s, e;

	for(s = 0; s < N; s++){
		if(s*h <= start && start < (s+1)*h) 
			break;
	}
	for(e = 0; e < N; e++){
		if(e*h <= end && end < (e+1)*h) 
			break;
	}

	for(int i = s; i <= e; i++) {
		if(i == s || i == e) 
			area += A[i].y;
		else 
			area += A[i].y*2;
	}

	area = area * h / 2;

	printf("*** Integration of pdf from %lf to %lf = %lf\n", start, end, area);
}


void program2_1()
{
	FILE *fp_r, *fp_w;
	struct _DATA *A;
	__int64 start, freq, end;
	float resultTime = 0;
	int N, ii;
	double h, integral_sum = 0.0;
	double test= 0.0;

	fp_r = fopen("sampling_table.txt", "r");		
	if(fp_r == NULL) {
		printf("input file not found....\n");
		exit(0);
	}

	fscanf(fp_r, "%d %lf", &N, &h);
	A = (struct _DATA*)malloc(sizeof(struct _DATA)*N);

	for( ii = 0;ii < N; ii++){
		fscanf(fp_r, "%lf %lf", &A[ii].x, &A[ii].y);

		if( ii == 0 || ii == N-1 )
			integral_sum += A[ii].y;
		else
			integral_sum += 2*A[ii].y;
	}

	h = (double)1/(N-1);

	integral_sum = h * integral_sum /2;

	for( ii = 0; ii < N; ii++){
		A[ii].x = h * ii;
		A[ii].y = A[ii].y / integral_sum;
	}

	for( ii = 0; ii < N; ii++){
		test += A[ii].y;
	}
	
	printArea(A, N, 0.0, 1.0);
	printArea(A, N, 0.0, 0.25);
	printArea(A, N, 0.25, 0.5);
	printArea(A, N, 0.25, 0.75);
	printArea(A, N, 0.75, 1.0);
		
	fp_w = fopen("pdf_table.txt", "w");
	fprintf(fp_w, "%d %.6lf\n", N, h);
	for(ii = 0; ii < N; ii++){
		fprintf(fp_w, "%.6lf %.6lf\n", A[ii].x, A[ii].y);
	}

	if(fp_r != NULL) fclose(fp_r);
	if(fp_w != NULL) fclose(fp_w);

	
	// Step 0: sampling_table.txt 파일의 내용을 입력 받는다.

	// Step 1: x의 구간을 [0, 1] 사이로 정규화한다.
	// 이때 정규화 하면서 x의 간격이 바뀌였기 때문에 나중에 적분시 사다리꼴의 높이가 바뀐 것을 주의해야 한다!!
	

	// Step 2: 곡선 함수를 확률밀도함수(pdf)로 만들기 위해 정규화한 x의 전체 구간에서 y값을 수치적으로 적분했을 때 1이 나오게 y값을 변환한다. 
	// (이제 이 곡선 함수는 확률밀도 함수의 조건을 만족했으므로 y를 p(x)라고 하겠다.)
	// y -> p(x)로 바꾸는 식은 강의자료 11쪽 참고 (분모의 x의 전체구간을 적분하는 수식은 합성 사다리꼴 공식을 사용하며 7쪽 3.2 부분 참고)
	

	// Step 3: 위에서 구한 x와 p(x) 값을 sampling_table.txt와 같은 포맷으로 pdf_table.txt에 저장한다.


	// Step 4: 위에서 구한 p(x)에 대해서 x의 전체구간 적분을 했을 때 1에 근사하게 나오는지 확인한다.
	// Console 창에 강의자료 12쪽 포맷을 참고해서 출력한다.
}