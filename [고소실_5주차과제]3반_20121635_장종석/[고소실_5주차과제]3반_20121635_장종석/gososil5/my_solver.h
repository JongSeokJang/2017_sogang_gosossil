#ifndef _MY_SOLVER
#define _MY_SOLVER

#include <stdio.h>
#include <Windows.h>

#define CHECK_TIME_START QueryPerformanceFrequency((_LARGE_INTEGER*) &freq); QueryPerformanceCounter((_LARGE_INTEGER*)&start);
#define CHECK_TIME_END(a) QueryPerformanceCounter((_LARGE_INTEGER*)&end); a = (float)((float) (end - start)/freq);

void program2_1(void);
void program2_2(void);

// HOMEWORK
void program2_2_a(void);
void program2_2_b(void);
void program2_3(void);

struct _DATA {
	double x;
	double y;
};

typedef double double_func(double);

double linear_interpolation(struct _DATA *F, int index, double x1);
double bisectin_method(struct _DATA *F, int N, double h, double U);
double secant_method(struct _DATA *F, int N, double h, double U);
double calculate_Area(struct _DATA *A, int N, double h, double start, double end);


double accumArea(double); // fast cumulative distribution function, discretely pre-calculated c.d.f should be stored at array F
double diffAccumArea(double); // differential of accumArea, it can be used as differential of _F2
double secantFunc(double_func*, double_func*, double); // newton function
double _F2(double); // return getAccumArea2(x) - U

#endif // _MY_SOLVER