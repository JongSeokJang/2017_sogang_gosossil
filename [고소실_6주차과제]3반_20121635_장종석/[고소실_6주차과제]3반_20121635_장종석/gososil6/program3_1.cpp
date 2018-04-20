#include "my_solver.h"
#include <stdlib.h>
#include <string.h>

#define SOLNUMS 4
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

double C, b, p[SOLNUMS][SOLNUMS], t[SOLNUMS], tr[SOLNUMS];

void hybrd1( int *n, double *x, double *fvec, int *iflag )
{
	fvec[0] = (x[0] - p[0][0]) * (x[0] - p[0][0]) + (x[1] - p[0][1]) * (x[1] - p[0][1]) + (x[2] - p[0][2]) * (x[2] - p[0][2]) - (C*(tr[0] + x[3] - t[0])) * (C*(tr[0] + x[3] - t[0]));
	fvec[1] = (x[0] - p[1][0]) * (x[0] - p[1][0]) + (x[1] - p[1][1]) * (x[1] - p[1][1]) + (x[2] - p[1][2]) * (x[2] - p[1][2]) - (C*(tr[1] + x[3] - t[1])) * (C*(tr[1] + x[3] - t[1]));
	fvec[2] = (x[0] - p[2][0]) * (x[0] - p[2][0]) + (x[1] - p[2][1]) * (x[1] - p[2][1]) + (x[2] - p[2][2]) * (x[2] - p[2][2]) - (C*(tr[2] + x[3] - t[2])) * (C*(tr[2] + x[3] - t[2]));
	fvec[3] = (x[0] - p[3][0]) * (x[0] - p[3][0]) + (x[1] - p[3][1]) * (x[1] - p[3][1]) + (x[2] - p[3][2]) * (x[2] - p[3][2]) - (C*(tr[3] + x[3] - t[3])) * (C*(tr[3] + x[3] - t[3]));
}

void hybrj1( int *n, double *x, double *fvec, double *fjac, int *ldfjac, int *iflag )
{
	// origin function F(x)
	if( *iflag == 1 ){
		fvec[0] = (x[0] - p[0][0]) * (x[0] - p[0][0]) + (x[1] - p[0][1]) * (x[1] - p[0][1]) + (x[2] - p[0][2]) * (x[2] - p[0][2]) - (C*(tr[0] + x[3] - t[0])) * (C*(tr[0] + x[3] - t[0]));
		fvec[1] = (x[0] - p[1][0]) * (x[0] - p[1][0]) + (x[1] - p[1][1]) * (x[1] - p[1][1]) + (x[2] - p[1][2]) * (x[2] - p[1][2]) - (C*(tr[1] + x[3] - t[1])) * (C*(tr[1] + x[3] - t[1]));
		fvec[2] = (x[0] - p[2][0]) * (x[0] - p[2][0]) + (x[1] - p[2][1]) * (x[1] - p[2][1]) + (x[2] - p[2][2]) * (x[2] - p[2][2]) - (C*(tr[2] + x[3] - t[2])) * (C*(tr[2] + x[3] - t[2]));
		fvec[3] = (x[0] - p[3][0]) * (x[0] - p[3][0]) + (x[1] - p[3][1]) * (x[1] - p[3][1]) + (x[2] - p[3][2]) * (x[2] - p[3][2]) - (C*(tr[3] + x[3] - t[3])) * (C*(tr[3] + x[3] - t[3]));
	}
	// Jacobi matrix J(x)
	else if( *iflag == 2 ){
		fjac[0] = 2*(x[0] - p[0][0]); fjac[4] = 2*(x[1] - p[0][1]); fjac[8]  = 2*(x[2] - p[0][2]); fjac[12] = -2*C*C*(tr[0] + x[3] - t[0]);
		fjac[1] = 2*(x[0] - p[1][0]); fjac[5] = 2*(x[1] - p[1][1]);	fjac[9]  = 2*(x[2] - p[1][2]); fjac[13] = -2*C*C*(tr[1] + x[3] - t[1]);
		fjac[2] = 2*(x[0] - p[2][0]); fjac[6] = 2*(x[1] - p[2][1]);	fjac[10] = 2*(x[2] - p[2][2]); fjac[14] = -2*C*C*(tr[2] + x[3] - t[2]);
		fjac[3] = 2*(x[0] - p[3][0]); fjac[7] = 2*(x[1] - p[3][1]);	fjac[11] = 2*(x[2] - p[3][2]); fjac[15] = -2*C*C*(tr[3] + x[3] - t[3]);
	}
}

void program3_1(void)
{	
	FILE *fp_r, *fp_w1, *fp_w2;
	int i;
	int n=SOLNUMS,ldfjac = MATROWS, info;
	int lwa_hybrj = (SOLNUMS*(SOLNUMS+13))/2;
	int lwa_hybrd = (SOLNUMS*(3*SOLNUMS+13))/2;

	double tol = TOLERANCE;
	double x_hybrj[SOLNUMS], x_hybrd[SOLNUMS], fvec[SOLNUMS], fjac[SOLNUMS*SOLNUMS];

	double *wa_hybrj;
	double *wa_hybrd;
	char rfname[64+1], wfname1[64+1], wfname2[64+1];
	
	for (i = 0; i < 3; i++) {

		wa_hybrj = (double*)malloc(sizeof(double) *lwa_hybrj);
		wa_hybrd = (double*)malloc(sizeof(double)* lwa_hybrd);
		
		sprintf(rfname, "GPS_signal_%d.txt",i);
		sprintf(wfname1, "GPS_position_3-1_%d.txt",i);
		sprintf(wfname2, "GPS_position_3-2_%d.txt",i);

		fp_r = fopen(rfname, "r");
		fp_w1 = fopen(wfname1, "w");
		fp_w2 = fopen(wfname2, "w");

		if( fp_r == NULL){
			printf("read file open error![%s]\n", rfname);
			return;
		}

		fscanf(fp_r, "%lf %lf", &C, &b);
		for( int j = 0; j < SOLNUMS; j++)
			fscanf(fp_r, "%lf %lf %lf %lf %lf", &p[0][j], &p[1][j], &p[2][j], &t[0], &tr[0]);

		printf("GPS Signal %d, input 4 values(x10, x20, x30, x40) : ", i);
		scanf("%lf %lf %lf %lf", &x_hybrj[0], &x_hybrj[1], &x_hybrj[2], &x_hybrj[3]);

		x_hybrd[0] = x_hybrj[0];
		x_hybrd[1] = x_hybrj[1];
		x_hybrd[2] = x_hybrj[2];
		x_hybrd[3] = x_hybrj[3];

		fprintf(fp_w1, "initial \tx1 : %lf, x2 : %lf, x3 : %lf, x4 : %lf\n", x_hybrj[0], x_hybrj[1], x_hybrj[2], x_hybrj[3]);
		fprintf(fp_w2, "initial \tx1 : %lf, x2 : %lf, x3 : %lf, x4 : %lf\n", x_hybrd[0], x_hybrd[1], x_hybrd[2], x_hybrd[3]);
		
		memset(wa_hybrj, 0, sizeof(wa_hybrj));
		memset(fvec, 0, sizeof(fvec));
		memset(fjac, 0, sizeof(fjac));
		hybrj1_(hybrj1, &n, x_hybrj, fvec, fjac, &ldfjac, &tol, &info, wa_hybrj, &lwa_hybrj);
		
		fprintf(fp_w1, "solution\tx1 : %lf, x2 : %lf, x3 : %lf, x4 : %lf\n", x_hybrj[0], x_hybrj[1], x_hybrj[2], x_hybrj[3]);
		fprintf(fp_w1, "confirm \tf1 : %lf, f2 : %lf, f3 : %lf, f4 : %lf\n", fvec[0], fvec[1], fvec[2], fvec[3]);

		memset(wa_hybrd, 0, sizeof(wa_hybrd));
		memset(fvec, 0, sizeof(fvec));
		hybrd1_(hybrd1, &n, x_hybrd, fvec, &tol, &info, wa_hybrd, &lwa_hybrd);

		/* write HYBRD1 result */
		fprintf(fp_w2, "solution\tx1 : %lf, x2 : %lf, x3 : %lf, x4 : %lf\n", x_hybrd[0], x_hybrd[1], x_hybrd[2], x_hybrd[3]);
		fprintf(fp_w2, "confirm \tf1 : %lf, f2 : %lf, f3 : %lf, f4 : %lf\n", fvec[0], fvec[1], fvec[2], fvec[3]);
		
		free(wa_hybrj);
		free(wa_hybrd);
		fclose(fp_r);
		fclose(fp_w1);
		fclose(fp_w2);

	}




}