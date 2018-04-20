#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <Windows.h>


#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start) 
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f))
double a, b, c;
double f(double x){
	return a*x*x + b*x + c;
}


void solve_1(void){

	double root_1 = (-b +sqrt(b*b -4*a*c)) / (2*a);
	double root_2 = (-b -sqrt(b*b -4*a*c)) / (2*a);
	
	printf("-------------solve_1-------------\n");
	printf("x1 : %20.8lf\n", root_1);
	printf("x2 : %20.8lf\n", root_2);
	printf("---------------------------------\n");
	printf("f(%20.8lf) = %12.8lf\n", root_1, f(root_1) );
	printf("f(%20.8lf) = %12.8lf\n", root_2, f(root_2) );
}

void solve_2(void){

	double root_1 = (-4*a*c) / ((b+sqrt(b*b -4*a*c)) * (2*a) );
	double root_2 = (-b -sqrt(b*b -4*a*c)) / (2*a);


	printf("-------------solve_2-------------\n");
	printf("x1 : %20.8lf\n", root_1);
	printf("x2 : %20.8lf\n", root_2);
	printf("---------------------------------\n");
	printf("f(%20.8lf) = %12.8lf\n", root_1, f(root_1) );
	printf("f(%20.8lf) = %12.8lf\n", root_2, f(root_2) );


}


int main(void){
	
	double temp1;
	printf(" insert a, b, c : ");
	scanf("%lf %lf %lf", &a, &b, &c);

	
	solve_1();
	printf("\n\n");
	solve_2();



}