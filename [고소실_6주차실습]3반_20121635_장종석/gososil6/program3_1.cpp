#include "my_solver.h"

void rpoly(const char *readfilename, const char *writefilename)
{
	int NCOEF, DEGREE;

	double *poly;
	double *zeror, *zeroi;
	long int fail;
	int i,j,k;	

	FILE *fp_r = fopen(readfilename, "r");
	if(fp_r == NULL) 
	{
		printf("%s file open error...\n", readfilename);
		return;
	}

	FILE *fp_w = fopen(writefilename, "w");
	if(fp_w == NULL) 
	{
		printf("%s file open error...\n", writefilename);
		return;
	}	
	// TODO
	// 파일에서 몇개인지 확인을 한후, poly 를 malloc 해주면된다.
	// poly 배열에 하나씩 넣기 시작한다.
	fscanf(fp_r, "%d", &DEGREE);
	NCOEF = DEGREE + 1;
	poly = (double*)malloc(sizeof(double) * NCOEF);
	zeror = (double*)malloc(sizeof(double) * DEGREE);
	zeroi = (double*)malloc(sizeof(double) * DEGREE);

	for( i = 0; i < NCOEF; i++){
		fscanf(fp_r, "%lf", &poly[i]);
	}

	rpoly_(poly, &DEGREE, zeror, zeroi, &fail);
	
	if( fail ){
		printf("wrong\n");
		fprintf(fp_w, "wrong\n");
		return;
	}

	fprintf(fp_w,"zeror:\n");
	for( i = 0 ; i < DEGREE ; i++ )
		fprintf(fp_w,"%20.15f ", zeror[i]);
	fprintf(fp_w,"\n");
	fprintf(fp_w,"zeroi:\n");
	for( i = 0 ; i < DEGREE ; i++ )
		fprintf(fp_w,"%20.15f ", zeroi[i]);
	fprintf(fp_w,"\n\n");
	
	
	// zeroi == 0 일때 zeorr 의 값으로 값을 비교하자.
	double value_x ;
	for( k = 0 ; k < DEGREE; k++){
		if(zeroi[k] == 0.0){
			value_x = zeror[k];
			//printf("zeroi: [%.15lf] // zeror: [%.15lf]\n",zeroi[k], zeror[k]);
		
			double sum = 0.0;
			double temp_sum = 1.0;
			for( i = 0; i < NCOEF; i++){
				temp_sum = 1.0;
				for( j = 0; j < DEGREE-i; j++){
					temp_sum *= value_x;
				}
				sum += temp_sum * poly[i];
			}
			fprintf(fp_w,"f(%.15lf) = %.15lf\n\n", value_x, sum);
			//printf("[%s][%.15lf]\n",readfilename, sum);
		}

	}

	free(zeroi);
	free(zeror);
	free(poly);

	if(fp_r != NULL) fclose(fp_r);
	if(fp_w != NULL) fclose(fp_w);
}

void program3_1()
{
	rpoly("polynomial_3-1_1.txt", "roots_1.txt");
	rpoly("polynomial_3-1_2.txt", "roots_2.txt");
	rpoly("polynomial_3-1_3.txt", "roots_3.txt");
	rpoly("polynomial_3-1_4.txt", "roots_4.txt");
	rpoly("polynomial_3-1_5.txt", "roots_5.txt");
	rpoly("polynomial_3-1_6.txt", "roots_6.txt");
}