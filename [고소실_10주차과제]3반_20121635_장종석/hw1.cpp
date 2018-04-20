#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <Windows.h>


#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start) 
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f))
int N;
double *_DATA;
static __int64 start, freq, end; 


double get_x_bar(){
	int ii;
	double temp = 0.0;
	
	for( ii = 0 ; ii < N; ii++){
		temp += _DATA[ii];
	}
	return temp/N;
}
double get_sigma2(){
	int ii;
	double temp = 0.0;
	double x_bar = get_x_bar();
	
	for ( ii = 0 ; ii < N; ii++){
		temp += (_DATA[ii]-x_bar) * (_DATA[ii]-x_bar);
	}
	return temp/(N-1);
}

double get_sigma1(){
	int ii;
	double temp1 = 0.0;
	double temp2 = 0.0;


	for( ii = 0 ; ii < N; ii++){
		temp1 += _DATA[ii] * _DATA[ii];
		temp2 += _DATA[ii];
	}
	
	temp1 = temp1 * N;
	temp2 = temp2 * temp2;
	
	return (temp1-temp2) /(N*(N-1));

	
}

void init_hw1(){
	int ii;

	_DATA = (double*)malloc(sizeof(double) * N);
	srand((unsigned) time(NULL));
	for ( ii = 0; ii < N; ii++){
		_DATA[ii] = (double)rand()/ ((double)RAND_MAX);
	}

}
void free_hw1(){
	free(_DATA);
}


int main(){

	float time;
	double temp;
	int ii, jj;
	for( ii = 100, jj = 1 ; ii < 1000000000; ii *= 10, jj++){
		temp  = 0.0;
		N = ii;
		init_hw1();
		printf("----------[%d, (%d)]---------\n", jj, ii);
		CHECK_TIME_START;
		temp = get_sigma1();
		CHECK_TIME_END(time);
		printf("sig1 : %-25lf [%fs]\n", temp, time/1000.0);
		

		CHECK_TIME_START;
		temp = get_sigma2();
		CHECK_TIME_END(time);
		printf("sig2 : %-25lf [%fs]\n", temp, time/1000.0);
		
		
		free_hw1();
	}
}