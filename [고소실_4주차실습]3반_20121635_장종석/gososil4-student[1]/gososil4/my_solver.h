#ifndef _MY_SOLVER
#define _MY_SOLVER

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

#define DELTA 0.000001
#define Nmax 50
#define EPSILON 0.00001

double _F1(double x);
double _FP1(double x);
double _F2(double x);
double _FP2(double x);
double _F3(double x);
double _FP3(double x);
double _F4(double x);
double _FP4(double x);


#endif ///_MY_SOLVER