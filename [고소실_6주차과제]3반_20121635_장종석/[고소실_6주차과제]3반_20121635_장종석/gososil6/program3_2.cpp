#include "my_solver.h"

#define SOLNUMS 4
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001


void fcn3_2( int *n, double *x, double *fvec, int *iflag )
{	
	fvec[0] = 1.0*x[0] + 10.0*x[1] + 9.0;
	fvec[1] = sqrt(5.0)*(x[2] - x[3]) -2.0*sqrt(5.0);
	fvec[2] = (x[1] - 2.0*x[2]) * (x[1] - 2.0*x[2]) - 9.0;
	fvec[3] = sqrt(10.0)*(x[0]-x[3])*(x[0]-x[3]) -2*sqrt(10.0);

}

void program3_2(void)
{
	int n = SOLNUMS;
	double x[SOLNUMS] = {0.9, -0.9, 1.25, -1.25};	

	double fvec[SOLNUMS];	
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (3 *SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (3 *SOLNUMS + 13)) / 2;
	FILE * fp_w;


	fp_w = fopen("roots_found_3-2.txt", "w");

	fprintf(fp_w, "inital    \tx : %lf, y: %lf z:%lf, w:%lf\n", x[0], x[1], x[2], x[3]);
	hybrd1_( fcn3_2, &n, x, fvec, &tol, &info, wa, &lwa);

	fprintf(fp_w, "solutioin \tx : %lf, y: %lf z:%lf, w:%lf\n", x[0], x[1], x[2], x[3]); 
	fprintf(fp_w, "confirm   \tf1: %lf, f2: %lf, f3: %lf f4: %lf\n",fvec[0],fvec[1],fvec[2],fvec[3]);

	fclose(fp_w);

}