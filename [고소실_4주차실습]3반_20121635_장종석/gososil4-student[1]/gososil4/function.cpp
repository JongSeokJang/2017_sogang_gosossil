#include "my_solver.h"
#define M_PI 3.141592

//½Ç½À¿ë
double _F1(double x) {
	return x*x - 4*x + 4 - log(x);
}

double _FP1(double x) {
	return 2*x - 4 - (1/x);
}

double _F2(double x) {
	return x + 1 - 2*sin(M_PI*x);
}

double _FP2(double x) {
	return 1 - 2*M_PI*cos(M_PI*x);
}

double _F3(double x) {
	return x*x*x*x - 11.0*x*x*x + 42.35*x*x -66.55*x +35.1384;
}

double _FP3(double x) {
	return 4*x*x*x - 33.0*x*x + 84.70*x -66.55;
}

double _F4(double x) {
	return x*x -2;
}

double _FP4(double x) {
	return 2*x;
}