#include "my_solver.h"

extern double (*_F)(double);
extern double (*_FP)(double);

/*********************************************
  Newton-Rapson Method
**********************************************/
void program1_1(FILE *fp)
{
	double x0 = DBL_MAX;
	double x1;
	int n;

	fprintf(fp, " n              xn1                  |f(xn1)|\n");
	scanf("%lf", &x1);

	for(n=0; ;n++) {		
		
		if( fabs(_F(x1)) <  DELTA || n >= Nmax || fabs(x1 - x0) <EPSILON)
			break;

		fprintf(fp, "%2d  %20.18e  %12.10e\n", n, x1, fabs(_F(x1)));

		// TODO
		x0 = x1;
		x1 = x1 - _F(x1)/_FP(x1);

		
		
	}
	printf("%2d  %20.18e  %12.10e\n", n, x1, fabs(_F(x1)));
}