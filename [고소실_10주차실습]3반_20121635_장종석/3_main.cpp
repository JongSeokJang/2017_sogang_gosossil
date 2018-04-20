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
		메뉴얼에서 제공하는 방법으로 계산을 한경우, 
		4.872486e-003의 결과나 나왔다. 이 경우 문제의 답인 2.485168 * 10^-4과 다소 차이가 존재한다.
		이 것은 음수를 계산할 경우 메뉴얼에서 제공한 방법대로 진행할 경우, 작은 값을 서로 빼다보니,
		float 특성상, 조금씩 값에 오차가 발생하여 생기는 문제이다.
		이를 해결하기 위하여 Taylor_series_ex()에서 주어진 x의 값이 양수일경우는 Taylor_series(x)
		를 계산하면 값이 나오고, 만약 음수일 경우, 1/Taylor_series(-x)의 값을 사용하였다.
		이로 인하여 알 수 있는 것은, 작은 수를 뺄 경우, 빼기 연산을 사용하는것보다
		이를 다른 연산으로 변환하여 계산을해야 정확한 값이 도출됨을 알 수 있었다.
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
