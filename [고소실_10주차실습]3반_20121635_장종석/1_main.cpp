#include <stdio.h>
#include <random>
#include <time.h>

#include <math.h>
#include <time.h>
#include <Windows.h>

__int64 start, freq, end;
#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f))
float compute_time;


#define MATDIM 1024
double *pMatA, *pMatB, *pMatC;
void MultiplySquareMatrices_1(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize);
void MultiplySquareMatrices_2(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize);
void MultiplySquareMatrices_3(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize);
void MultiplySquareMatrices_4(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize);

void init_MatMat(void);


void main(void)
{


	/*	
		�ǽ��� ���, 1���� �ܼ��� 2���� ����� ���� ���ϴ� ��������,
		2���� ��� 1���� �ð��� ���̴� ����� ��ġ����� ����ϴ� ����̾���.
		2��������� ���� AB�� �Ҷ� B�� ��� memory �� ŭ�����ϰ� �Ѿ�� ������
		memory ��꿡 �־ �ð��� ���� �����ϰ� �Ǿ���. �̸� �����ϱ� ���Ͽ�
		��ġ����� ���� ��, ����� �Ͽ����� 1���� ���ؼ� �ӵ��� ���� ���� ���� 
		Ȯ���� �� �־���. �ǽ� 3���� ���, intel ���� �����ϴ� �������� 8���� 
		loop unrolling ���������� ��� �ӵ��� ���� �����ٰ� �Ͽ� ���� ���Ҵ���
		���� ���� ��츦 Ȯ���� �� �־���. 4���� ��� Ȥ�� 6�� �ƴ϶� �ٸ� ���� �������
		�� ���� ��찡 �����Ҽ� ������?? ��� �ǹ����� �ٸ� ���ڸ� �Ͽ��µ�
		�ϴ��� 2�� �ŵ������� �ϳ��� 64�� �Ͽ���. �� ��� 8���� �� �ͺ��ٴ� 
		�ణ �������� Ȯ���Ͽ���. ��, ���� ȿ������ ������� �Ϸ���, ��ġ����� ����
		loop unrolling ����� 8���� ����ϴ� ���� ����� ���� ���� ���� �����
		��Ÿ ������.
	*/

	init_MatMat();

	
	CHECK_TIME_START;
	//Sleep(500);
	MultiplySquareMatrices_1(pMatC, pMatA, pMatB, MATDIM);
	CHECK_TIME_END(compute_time);
	printf("None Fun Time = %f ms\n", compute_time);
	
	
	CHECK_TIME_START;
	//Sleep(500);
	MultiplySquareMatrices_2(pMatC, pMatA, pMatB, MATDIM);
	CHECK_TIME_END(compute_time);
	printf("None Fun Time = %f ms\n", compute_time);
	
	
	CHECK_TIME_START;
	//Sleep(500);
	MultiplySquareMatrices_3(pMatC, pMatA, pMatB, MATDIM);
	CHECK_TIME_END(compute_time);
	printf("None Fun Time = %f ms\n", compute_time);

	
	CHECK_TIME_START;
	//Sleep(500);
	MultiplySquareMatrices_4(pMatC, pMatA, pMatB, MATDIM);
	CHECK_TIME_END(compute_time);
	printf("None Fun Time = %f ms\n", compute_time);
	

}

void MultiplySquareMatrices_1(double *pDestMatrix, double *pLeftMatrix ,double *pRightMatrix, int MatSize)
{
		
	int i, j, k;
	for(i = 0; i < MatSize; i++) {

		for(j = 0; j < MatSize; j++) {

			pDestMatrix[i*MatSize+j] = 0.0;
			for(k = 0; k < MatSize; k++)
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k] * pRightMatrix[k*MatSize+j];
		}
	}
	
}

void MultiplySquareMatrices_2(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize)
{

	int i, j, k;
	double *temp = (double *)malloc(sizeof(double) *MATDIM*MATDIM);
	memset(temp, 0x00, sizeof(temp));

	for( i = 0; i < MatSize; i++){
		for( j = 0; j < MATDIM; j++){
			temp[i*MATDIM + j] = pRightMatrix[i + j*MATDIM];
		}
	}

	for( i = 0 ; i < MatSize; i++){
		
		for (j = 0; j < MatSize;j++){
			pDestMatrix[i*MatSize + j] = 0;
			for ( k = 0 ; k < MatSize; k++){
				pDestMatrix[i*MatSize + j] += pLeftMatrix[i*MATDIM+k] + temp[j*MATDIM+k];
			}
		}
	}

	free (temp);
	


}
void MultiplySquareMatrices_3(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize)
{
	int i, j, k;
	double *temp = (double *)malloc(sizeof(double) *MATDIM*MATDIM);
	memset(temp, 0x00, sizeof(temp));

	for( i = 0; i < MatSize; i++){
		for( j = 0; j < MATDIM; j++){
			temp[i*MATDIM + j] = pRightMatrix[i + j*MATDIM];
		}
	}

	for(i = 0; i < MatSize; i++) {
		for(j = 0; j < MatSize; j++) {

			pDestMatrix[i*MatSize+j] = 0.0;
			for(k = 0; k < MatSize / 8; k++) {
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*8] * temp[j*MatSize+k*8];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*8+1] * temp[j*MatSize+k*8+1];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*8+2] * temp[j*MatSize+k*8+2];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*8+3] * temp[j*MatSize+k*8+3];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*8+4] * temp[j*MatSize+k*8+4];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*8+5] * temp[j*MatSize+k*8+5];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*8+6] * temp[j*MatSize+k*8+6];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*8+7] * temp[j*MatSize+k*8+7];
			}
			for(k = 0; k < MatSize % 8; k++)
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*(MatSize/8)*8+k] * temp[j*(MatSize/8)*8+k];
		}
	}
	free(temp);

}
void MultiplySquareMatrices_4(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize)
{
	int i, j, k;
	double *temp = (double *)malloc(sizeof(double) *MATDIM*MATDIM);
	memset(temp, 0x00, sizeof(temp));

	for( i = 0; i < MatSize; i++){
		for( j = 0; j < MATDIM; j++){
			temp[i*MATDIM + j] = pRightMatrix[i + j*MATDIM];
		}
	}

	for(i = 0; i < MatSize; i++) {
		for(j = 0; j < MatSize; j++) {
			pDestMatrix[i*MatSize+j] = 0.0;
			for(k = 0; k < MatSize / 64; k++) {
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64] * temp[j*MatSize+k*64];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+1] * temp[j*MatSize+k*64+1];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+2] * temp[j*MatSize+k*64+2];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+3] * temp[j*MatSize+k*64+3];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+4] * temp[j*MatSize+k*64+4];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+5] * temp[j*MatSize+k*64+5];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+6] * temp[j*MatSize+k*64+6];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+7] * temp[j*MatSize+k*64+7];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+8] * temp[j*MatSize+k*64+8];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+9] * temp[j*MatSize+k*64+9];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+10] * temp[j*MatSize+k*64+10];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+11] * temp[j*MatSize+k*64+11];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+12] * temp[j*MatSize+k*64+12];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+13] * temp[j*MatSize+k*64+13];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+14] * temp[j*MatSize+k*64+14];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+15] * temp[j*MatSize+k*64+15];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+16] * temp[j*MatSize+k*64+16];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+17] * temp[j*MatSize+k*64+17];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+18] * temp[j*MatSize+k*64+18];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+19] * temp[j*MatSize+k*64+19];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+20] * temp[j*MatSize+k*64+20];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+21] * temp[j*MatSize+k*64+21];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+22] * temp[j*MatSize+k*64+22];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+23] * temp[j*MatSize+k*64+23];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+24] * temp[j*MatSize+k*64+24];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+25] * temp[j*MatSize+k*64+25];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+26] * temp[j*MatSize+k*64+26];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+27] * temp[j*MatSize+k*64+27];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+28] * temp[j*MatSize+k*64+28];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+29] * temp[j*MatSize+k*64+29];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+30] * temp[j*MatSize+k*64+30];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+31] * temp[j*MatSize+k*64+31];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+32] * temp[j*MatSize+k*64+32];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+33] * temp[j*MatSize+k*64+33];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+34] * temp[j*MatSize+k*64+34];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+35] * temp[j*MatSize+k*64+35];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+36] * temp[j*MatSize+k*64+36];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+37] * temp[j*MatSize+k*64+37];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+38] * temp[j*MatSize+k*64+38];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+39] * temp[j*MatSize+k*64+39];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+40] * temp[j*MatSize+k*64+40];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+41] * temp[j*MatSize+k*64+41];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+42] * temp[j*MatSize+k*64+42];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+43] * temp[j*MatSize+k*64+43];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+44] * temp[j*MatSize+k*64+44];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+45] * temp[j*MatSize+k*64+45];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+46] * temp[j*MatSize+k*64+46];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+47] * temp[j*MatSize+k*64+47];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+48] * temp[j*MatSize+k*64+48];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+49] * temp[j*MatSize+k*64+49];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+50] * temp[j*MatSize+k*64+50];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+51] * temp[j*MatSize+k*64+51];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+52] * temp[j*MatSize+k*64+52];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+53] * temp[j*MatSize+k*64+53];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+54] * temp[j*MatSize+k*64+54];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+55] * temp[j*MatSize+k*64+55];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+56] * temp[j*MatSize+k*64+56];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+57] * temp[j*MatSize+k*64+57];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+58] * temp[j*MatSize+k*64+58];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+59] * temp[j*MatSize+k*64+59];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+60] * temp[j*MatSize+k*64+60];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+61] * temp[j*MatSize+k*64+61];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+62] * temp[j*MatSize+k*64+62];
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*MatSize+k*64+63] * temp[j*MatSize+k*64+63];
			}
			for(k = 0; k < MatSize % 64; k++)
				pDestMatrix[i*MatSize+j] += pLeftMatrix[i*(MatSize/64)*64+k] * temp[j*(MatSize/64)*64+k];
		}
	}
	free(temp);
}




void init_MatMat(void)
{
	double *ptr;
	pMatA = (double *)malloc(sizeof(double)*MATDIM*MATDIM);
	pMatB = (double *)malloc(sizeof(double)*MATDIM*MATDIM);
	pMatC = (double *)malloc(sizeof(double)*MATDIM*MATDIM);
	srand((unsigned)time(NULL));
	ptr = pMatA;
	for (int i = 0; i < MATDIM*MATDIM; i++)
		*ptr++ = (double)rand() / ((double)RAND_MAX);
	ptr = pMatB;
	for (int i = 0; i < MATDIM*MATDIM; i++)
		*ptr++ = (double)rand() / ((double)RAND_MAX);
}
