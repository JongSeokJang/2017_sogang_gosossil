#include "my_solver.h"

extern double (*_F)(double);
extern double (*_FP)(double);

/*********************************************
  Bisection Method -- HOMEWORK
**********************************************/
void program1_3(FILE *fp)
{
	double x0 = DBL_MAX;
	double a0, b0, x1, temp;
	int n;

	fprintf(fp, " n              xn1                  |f(xn1)|\n");
	scanf("%lf %lf", &a0, &b0);
	x1 = (a0 + b0)/2.0;
	for(n=0; ;n++){
		fprintf(fp, "%2d  %20.18e  %12.10e\n", n, x1, fabs(_F(x1)));
		
		// TODO
		break;
	}
	printf("%2d  %20.18e  %12.10e\n", n, x1, fabs(_F(x1)));
}