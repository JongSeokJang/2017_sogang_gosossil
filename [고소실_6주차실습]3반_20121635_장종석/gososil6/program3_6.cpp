#include "my_solver.h"
#include <math.h>


#define SOLNUMS 2
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

struct xy_value{
	double x;
	double y;
	double fvec_x;
	double fvec_y;
};


void fcn3_6( int *n, double *x, double *fvec, int *iflag )
{	
	// x[0] is x;
	// x[1] is y;
	fvec[0] = 2.0*x[0]*x[0]*x[0] - 12.0*x[0] - x[1] - 1.0;
	fvec[1] = 3.0*x[1]*x[1] - 6.0*x[1] - x[0] - 3.0;		
}

void program3_6(void)
{
	//TODO
	struct xy_value solve[10];
	int i, j,k ,l = 0;
	int n = SOLNUMS;
	double x[SOLNUMS] = {0.1, 1.2};	//need to initilize x0

	double fvec[SOLNUMS];	
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (3 *SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (3 *SOLNUMS + 13)) / 2;
	FILE *fp_w = fopen("roots_3-5.txt", "w");
	
	for( i = -4; i <= 4; i++){
		for( j = -5; j <= 5; j++){
			x[0] = i;
			x[1] = j;
			hybrd1_( fcn3_6, &n, x, fvec, &tol, &info, wa, &lwa);
	
			if( l == 0 ){
				solve[l].x = x[0];
				solve[l].y = x[1];
				solve[l].fvec_x = fvec[0];
				solve[l].fvec_y = fvec[1];
				l++;
			}
			else{

				for( k = 0; k < l; k++){
					if(solve[k].x - x[0] >-0.00000001 && solve[k].x - x[0] < 0.00000001)
						break;

					if(k == l-1){
						solve[l].x = x[0];
						solve[l].y = x[1];
						solve[l].fvec_x = fvec[0];
						solve[l].fvec_y = fvec[1];
						l++;
					}
				}
			}
			
		}
	}
	// 근
	fprintf(fp_w, "zeror\n");
	for( i =0; i< 6; i++)
		fprintf(fp_w,"(%lf, %lf)\n", solve[i].x, solve[i].y);
	
	fprintf(fp_w, "\n");
	// 근이 맞는지 확인.
	for( i = 0 ; i < 6; i++){
		fprintf(fp_w,"f(%.10lf,%.10lf)	= (%lf,	%lf) \n", solve[i].x, solve[i].y, solve[i].fvec_x, solve[i].fvec_y);
	}
	
}