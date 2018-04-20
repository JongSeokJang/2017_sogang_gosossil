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

	
	// Step 0: sampling_table.txt ������ ������ �Է� �޴´�.

	// Step 1: x�� ������ [0, 1] ���̷� ����ȭ�Ѵ�.
	// �̶� ����ȭ �ϸ鼭 x�� ������ �ٲ�� ������ ���߿� ���н� ��ٸ����� ���̰� �ٲ� ���� �����ؾ� �Ѵ�!!
	

	// Step 2: � �Լ��� Ȯ���е��Լ�(pdf)�� ����� ���� ����ȭ�� x�� ��ü �������� y���� ��ġ������ �������� �� 1�� ������ y���� ��ȯ�Ѵ�. 
	// (���� �� � �Լ��� Ȯ���е� �Լ��� ������ ���������Ƿ� y�� p(x)��� �ϰڴ�.)
	// y -> p(x)�� �ٲٴ� ���� �����ڷ� 11�� ���� (�и��� x�� ��ü������ �����ϴ� ������ �ռ� ��ٸ��� ������ ����ϸ� 7�� 3.2 �κ� ����)
	

	// Step 3: ������ ���� x�� p(x) ���� sampling_table.txt�� ���� �������� pdf_table.txt�� �����Ѵ�.


	// Step 4: ������ ���� p(x)�� ���ؼ� x�� ��ü���� ������ ���� �� 1�� �ٻ��ϰ� �������� Ȯ���Ѵ�.
	// Console â�� �����ڷ� 12�� ������ �����ؼ� ����Ѵ�.
}