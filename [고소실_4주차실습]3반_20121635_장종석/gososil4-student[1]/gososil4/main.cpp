#include "my_solver.h"

extern void program1_1(FILE *fp); // Newton-Raphson Method
extern void program1_2(FILE *fp); // Secant Method
extern void program1_3(FILE *fp); // Bisection Method

double (*_F)(double);
double (*_FP)(double);

void main()
{
	FILE *fp;

	fp = fopen("result.txt", "w");

	/**********************
	  Problem 1-1
	 **********************/
	// f1 = x^2 - 4x + 4 - lnx = 0

	_F = _F1;
	_FP = _FP1;

	program1_1(fp);
	program1_2(fp);


	// f2 = x + 1 - 2sin(PI*x) = 0

	_F = _F2;
	_FP = _FP2;

	program1_1(fp);
	program1_2(fp);


	/**********************
	  Problem 1-2
	**********************/
	// f3 = x^4 - 11.0x^3 + 42.35x^2 - 66.55x + 35.1384 = 0

	_F = _F3;
	_FP = _FP3;

	for(int i = 3; i >= 0; i--)
		program1_1(fp);
	

	/**********************
	Problem 1-3
	**********************/
	// f4 = x^2 - 2 = 0

	_F = _F4;
	_FP = _FP4;


	/**********************
	  Problem 2  -- HOMEWORK
	**********************/
	
	_F = _F1;
	program1_3(fp);

	_F = _F2;
	program1_3(fp);

	_F = _F3;
	program1_3(fp);
	
	fclose(fp);	
}