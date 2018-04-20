#include <stdio.h>
#include <random>
#include <time.h>
#include <math.h>
#include <time.h>
#include <Windows.h>

#define N 24
double Taylor_series(double x, int n);
double Taylor_series_ex(double x, int n);
__int64 start, freq, end;
#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f))
float compute_time;

void main(void)
{
	/*
		�޴��󿡼� �����ϴ� ������� ����� �Ѱ��, 
		4.872486e-003�� ����� ���Դ�. �� ��� ������ ���� 2.485168 * 10^-4�� �ټ� ���̰� �����Ѵ�.
		�� ���� ������ ����� ��� �޴��󿡼� ������ ������ ������ ���, ���� ���� ���� ���ٺ���,
		float Ư����, ���ݾ� ���� ������ �߻��Ͽ� ����� �����̴�.
		�̸� �ذ��ϱ� ���Ͽ� Taylor_series_ex()���� �־��� x�� ���� ����ϰ��� Taylor_series(x)
		�� ����ϸ� ���� ������, ���� ������ ���, 1/Taylor_series(-x)�� ���� ����Ͽ���.
		�̷� ���Ͽ� �� �� �ִ� ����, ���� ���� �� ���, ���� ������ ����ϴ°ͺ���
		�̸� �ٸ� �������� ��ȯ�Ͽ� ������ؾ� ��Ȯ�� ���� ������� �� �� �־���.
	*/


	printf("** PROBLEM 3 **\n");
	printf("*** f(-8.3) = %e\n", Taylor_series(-8.3, 24));
	printf("*** f(-8.3) = %e\n", Taylor_series_ex(-8.3, 200));
}

double Taylor_series(double x, int n)
{
	int ii;
	double res = 1.0;
	
	for ( ii = n ; ii >= 1 ; ii--){
		res = res * (x / (double)ii ) + 1;
	}
	return res;
	
}
double Taylor_series_ex(double x, int n)
{

	int ii;
	double res;
	if( x < 0 ){
		res = Taylor_series(-x, n);
		return 1/res;
	}
	else{
		res = Taylor_series(x, n);
		return res;
	}
	

}
