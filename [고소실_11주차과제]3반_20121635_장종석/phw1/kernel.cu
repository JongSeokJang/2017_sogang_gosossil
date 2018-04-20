
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <Windows.h>
#include <assert.h>

#define CUDA_CALL(x) { const cudaError_t a = (x); if(a != cudaSuccess) { printf("\nCuda Error: %s (err_num=%d) at line:%d\n", cudaGetErrorString(a), a, __LINE__); cudaDeviceReset(); assert(0);}}
typedef float TIMER_T;

#define USE_CPU_TIMER 1
#define USE_GPU_TIMER 1

#if USE_CPU_TIMER == 1
__int64 start, freq, end;
#define CHECK_TIME_START() { QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start); }
#define CHECK_TIME_END(a) { QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f)); }
#else
#define CHECK_TIME_START()
#define CHECK_TIME_END(a)
#endif

#if USE_GPU_TIMER == 1
cudaEvent_t cuda_timer_start, cuda_timer_stop;
#define CUDA_STREAM_0 (0)

void create_device_timer()
{
	CUDA_CALL(cudaEventCreate(&cuda_timer_start));
	CUDA_CALL(cudaEventCreate(&cuda_timer_stop));
}

void destroy_device_timer()
{
	CUDA_CALL( cudaEventDestroy( cuda_timer_start ) );
	CUDA_CALL( cudaEventDestroy( cuda_timer_stop ) );
}

inline void start_device_timer()
{
	cudaEventRecord(cuda_timer_start, CUDA_STREAM_0);
}

inline TIMER_T stop_device_timer()
{
	TIMER_T ms;
	cudaEventRecord(cuda_timer_stop, CUDA_STREAM_0);
	cudaEventSynchronize(cuda_timer_stop);

	cudaEventElapsedTime(&ms, cuda_timer_start, cuda_timer_stop);
	return ms;
}

#define CHECK_TIME_INIT_GPU() { create_device_timer(); }
#define CHECK_TIME_START_GPU() { start_device_timer(); }
#define CHECK_TIME_END_GPU(a) { a = stop_device_timer(); }
#define CHECK_TIME_DEST_GPU() { destroy_device_timer(); }
#else
#define CHECK_TIME_INIT_GPU()
#define CHECK_TIME_START_GPU()
#define CHECK_TIME_END_GPU(a)
#define CHECK_TIME_DEST_GPU()
#endif


#define BLOCK_SIZE 32
#define DELTA 0.000001
int n;
TIMER_T compute_time = 0;
TIMER_T device_time = 0;

__host__ void cuda_error_check(const char * prefix, const char * postfix)
{
	if (cudaPeekAtLastError() != cudaSuccess)
	{
		printf("%s%s%s", prefix, cudaGetErrorString(cudaGetLastError()), postfix);
		cudaDeviceReset();
		//wait_exit();
		exit(1);
	}
}

void find_roots_CPU(float *A, float *B, float *C, float *X0, float *X1, float *FX0, float *FX1, int n) {

	int i;
	float a, b, c, d, x0, x1, tmp;

	for(i = 0; i < n; i++) {
		a = A[i]; 
		b = B[i]; 
		c = C[i];

		d = sqrtf(b*b - 4.0f*a*c);
		tmp = 1.0f / (2.0f*a);
		x0 = (-b - d) * tmp;
		x1 = (-b + d) * tmp;

		if(x0 <= x1) {

			X0[i] = x0; 
			X1[i] = x1;
			FX0[i] = (a*x0 + b)*x0 + c;
			FX1[i] = (a*x1 + b)*x1 + c;

		} else {
			X0[i] = x1; 
			X1[i] = x0;
			FX0[i] = (a*x1 + b)*x1 + c;
			FX1[i] = (a*x0 + b)*x0 + c;

		}
	}
}


__global__ void find_roots_Kernel(float *A, float *B, float *C, float *X0, float *X1, float *FX0, float *FX1)
{
	
	unsigned int row = blockDim.y * blockIdx.y + threadIdx.y;
	unsigned int col = blockDim.x * blockIdx.x + threadIdx.x;

	unsigned int tid = gridDim.x * blockDim.x * row + col;
	
	float a, b, c, d, x0, x1, tmp;

	a = A[tid]; 
	b = B[tid]; 
	c = C[tid];

	d = sqrtf(b*b - 4.0f*a*c);
	tmp = 1.0f / (2.0f*a);
	x0 = (-b - d) * tmp;
	x1 = (-b + d) * tmp;
	if(x0 <= x1) {
		X0[tid] = x0; X1[tid] = x1;
		FX0[tid] = (a*x0 + b)*x0 + c;
		FX1[tid] = (a*x1 + b)*x1 + c;
	} else {
		X0[tid] = x1; X1[tid] = x0;
		FX0[tid] = (a*x1 + b)*x1 + c;
		FX1[tid] = (a*x0 + b)*x0 + c;
	}

}


void find_roots_GPU(float *A, float *B, float *C, float *X0, float *X1, float *FX0, float *FX1) 
{
	int n1, n2;
	float *d_A, *d_B, *d_C, *d_X0, *d_X1, *d_FX0, *d_FX1;
	size_t size;

	size = n*sizeof(float);

	CUDA_CALL( cudaMalloc(&d_A, size) );
	CUDA_CALL( cudaMalloc(&d_B, size) );
	CUDA_CALL( cudaMalloc(&d_C, size) );
	CUDA_CALL( cudaMalloc(&d_X0, size) );
	CUDA_CALL( cudaMalloc(&d_X1, size) );
	CUDA_CALL( cudaMalloc(&d_FX0, size) );
	CUDA_CALL( cudaMalloc(&d_FX1, size) );

	CUDA_CALL( cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice));
	CUDA_CALL( cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice));
	CUDA_CALL( cudaMemcpy(d_C, C, size, cudaMemcpyHostToDevice));

	n1 = 1024;
	n2 = n/1024;

	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	dim3 dimGrid( n1/dimBlock.x, n2/dimBlock.y);
	
	CHECK_TIME_INIT_GPU()
	CHECK_TIME_START_GPU()
	find_roots_Kernel <<< dimGrid, dimBlock >>> (d_A, d_B, d_C, d_X0, d_X1, d_FX0, d_FX1);
	CHECK_TIME_END_GPU(device_time);
	CHECK_TIME_DEST_GPU()

	CUDA_CALL( cudaMemcpy( X0, d_X0, size, cudaMemcpyDeviceToHost))
	CUDA_CALL( cudaMemcpy( X1, d_X1, size, cudaMemcpyDeviceToHost))
	CUDA_CALL( cudaMemcpy( FX0, d_FX0, size, cudaMemcpyDeviceToHost))
	CUDA_CALL( cudaMemcpy( FX1, d_FX1, size, cudaMemcpyDeviceToHost))

}

void main(void) {

	FILE *fp_A = fopen("A.bin", "rb");
	FILE *fp_B = fopen("B.bin", "rb");
	FILE *fp_C = fopen("C.bin", "rb");

	if(fp_A == NULL || fp_B == NULL || fp_C == NULL ) {
		printf(" - Cannot open abc.bin\n\n");
		return;
	}

	fread(&n, sizeof(int), 1, fp_A);
	fread(&n, sizeof(int), 1, fp_B);
	fread(&n, sizeof(int), 1, fp_C);

	float* A = new float[n];
	float* B = new float[n];
	float* C = new float[n];

	fread(A, sizeof(float), n, fp_A);
	fread(B, sizeof(float), n, fp_B);
	fread(C, sizeof(float), n, fp_C);

	float *X0 = new float[n];
	float *X1 = new float[n];
	float *FX0 = new float[n];
	float *FX1 = new float[n];

	float *X0_G = new float[n];
	float *X1_G = new float[n];
	float *FX0_G = new float[n];
	float *FX1_G = new float[n];

	CHECK_TIME_START();
	find_roots_CPU(A, B, C, X0, X1, FX0, FX1, n);
	CHECK_TIME_END( compute_time );
	printf("***CPU [%lf] Time taken = %.6fms\n",X0[n-1], compute_time);
	CUDA_CALL(cudaSetDevice(0));
	find_roots_GPU(A, B, C, X0_G, X1_G, FX0_G, FX1_G);
	printf("***GPU [%lf] Time taken = %.6fms\n",X0_G[n-1], device_time);

	FILE* fp_x0 = fopen( "X0.bin", "wb" );
	FILE* fp_x1 = fopen( "X1.bin", "wb" );
	FILE* fp_fx0 = fopen( "FX0.bin", "wb" );
	FILE* fp_fx1 = fopen( "FX1.bin", "wb" );


	fwrite(X0_G, sizeof(float), n, fp_x0);
	fwrite(X1_G, sizeof(float), n, fp_x1);
	fwrite(FX0_G, sizeof(float), n, fp_fx0);
	fwrite(FX1_G, sizeof(float), n, fp_fx1);

	fclose( fp_A );
	fclose( fp_B );
	fclose( fp_C );

	fclose(fp_x0);
	fclose(fp_x1);
	fclose(fp_fx0);
	fclose(fp_fx1);

	delete[] A;
	delete[] B;
	delete[] C;
	delete[] X0;
	delete[] X1;
	delete[] FX0;
	delete[] FX1;

	delete[] X0_G;
	delete[] X1_G;
	delete[] FX0_G;
	delete[] FX1_G;

}
