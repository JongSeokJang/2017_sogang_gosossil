
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include <Windows.h>
#include <time.h>
#include <assert.h>

//1 exercise
//2 prac 1-3
//3 Fibonacci
//4 gen
#define prac 3


#define CUDA_CALL(x) { const cudaError_t a = (x); if(a != cudaSuccess) { printf("\nCuda Error: %s (err_num=%d) at line:%d\n", cudaGetErrorString(a), a, __LINE__); cudaDeviceReset(); assert(0);}}
typedef float TIMER_T;
#define USE_CPU_TIMER 1
#define USE_GPU_TIMER 1
#if USE_CPU_TIMER == 1
__int64 start, freq, end;
#define CHECK_TIME_START { QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start); }
#define CHECK_TIME_END(a) { QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f)); }
#else
#define CHECK_TIME_START
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
	CUDA_CALL(cudaEventDestroy(cuda_timer_start));
	CUDA_CALL(cudaEventDestroy(cuda_timer_stop));
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

TIMER_T compute_time = 0;
TIMER_T device_time = 0;

#if prac==1

typedef struct {
	int width;
	int height;
	float *elements;
} Array;


#define MAX_N_ELEMENTS	(1 << 20)

void generate_random_float_array(float *array, int n) {

	int i;

	for (i = 0; i < n; i++) {
		array[i] = 3.1415926f*((float)rand() / RAND_MAX);
	}
}
void combine_two_arrays(float *x, float *y, float *z, int n) {
	int i;

	for (i = 0; i < n; i++) {
		z[i] = 1.0f / (sin(x[i])*cos(y[i]) + cos(x[i])*sin(y[i]));
	}
}

__global__ void CombineTwoArrraysKernel(Array A, Array B, Array C) {
	int row = blockDim.y*blockIdx.y + threadIdx.y;
	int col = blockDim.x*blockIdx.x + threadIdx.x;
	int id = gridDim.x*blockDim.x*row + col;
	C.elements[id] = 1.0f / (sin(A.elements[id])*cos(B.elements[id])+ cos(A.elements[id])*sin(B.elements[id]));
}

cudaError_t combine_two_arrays_GPU(const Array A, const Array B, Array C);

int BLOCK_SIZE = 32;

int main()
{
	int n_elements;

	srand((unsigned int)time(NULL));
	n_elements = MAX_N_ELEMENTS;
	Array A, B, C, G;
	A.width = B.width = C.width = G.width =1024;
	A.height = B.height = C.height = G.height = MAX_N_ELEMENTS / 1024;

	A.elements = (float *)malloc(sizeof(float)*MAX_N_ELEMENTS);
	B.elements = (float *)malloc(sizeof(float)*MAX_N_ELEMENTS);
	C.elements = (float *)malloc(sizeof(float)*MAX_N_ELEMENTS);
	G.elements = (float *)malloc(sizeof(float)*MAX_N_ELEMENTS);
	generate_random_float_array(A.elements, MAX_N_ELEMENTS);
	generate_random_float_array(B.elements, MAX_N_ELEMENTS);
	
	CHECK_TIME_START;
	combine_two_arrays(A.elements, B.elements, C.elements, n_elements);
	CHECK_TIME_END(compute_time);

	printf("***GPU C[10] = %f/ Time taken = %.6fms\n", C.elements[10], compute_time);


	cudaError_t cudaStatus = combine_two_arrays_GPU(A, B, G);
	if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "combine_two_arrays_GPU failed!");
        return 1;
    }
	printf("***GPU G[10] = %f/ Time taken = %.6fms\n", G.elements[10], device_time);

    // cudaDeviceReset must be called before exiting in order for profiling and
    // tracing tools such as Nsight and Visual Profiler to show complete traces.
    cudaStatus = cudaDeviceReset();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceReset failed!");
        return 1;
    }

    return 0;
}
cudaError_t combine_two_arrays_GPU(const Array A, const Array B, Array C) {
	
	//아래 함수들을 사용하여 어떻게 하면 가급적 정확한 시간을 측정할 수 있을지 생각해볼 것.

	
	
	cudaError_t cudaStatus;
	// Choose which GPU to run on, change this on a multi-GPU system.
	cudaStatus = cudaSetDevice(0);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
		goto Error;
	}/////////////  if(cu.....  ==CUDA_CALL


	Array d_A, d_B, d_C;
	size_t size;

	d_A.width = A.width; d_A.height = A.height;
	size = A.width * A.height * sizeof(float);
	CUDA_CALL(cudaMalloc(&d_A.elements, size))
	
	CUDA_CALL(cudaMemcpy(d_A.elements, A.elements, size, cudaMemcpyHostToDevice))

	d_B.width = B.width; d_B.height = B.height;
	size = B.width * B.height * sizeof(float);
	CUDA_CALL(cudaMalloc(&d_B.elements, size))
	CUDA_CALL(cudaMemcpy(d_B.elements, B.elements, size, cudaMemcpyHostToDevice))

	d_C.width = C.width; d_C.height = C.height;
	size = C.width * C.height * sizeof(float);
	CUDA_CALL(cudaMalloc(&d_C.elements, size))

	// Assume that width and height are multiples of BLOCK SIZE.
	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	dim3 dimGrid(A.width / dimBlock.x, A.height / dimBlock.y);

	CHECK_TIME_INIT_GPU()
	CHECK_TIME_START_GPU()
	CombineTwoArrraysKernel <<< dimGrid, dimBlock >>> (d_A, d_B, d_C);
	CHECK_TIME_END_GPU(device_time)
	CHECK_TIME_DEST_GPU()

	CUDA_CALL(cudaGetLastError())

	// cudaDeviceSynchronize waits for the kernel to finish, and returns
	// any errors encountered during the launch.
	CUDA_CALL(cudaDeviceSynchronize())
	CUDA_CALL(cudaMemcpy(C.elements, d_C.elements, size, cudaMemcpyDeviceToHost))
	

Error:
	cudaFree(d_A.elements);
	cudaFree(d_B.elements);
	cudaFree(d_C.elements);
	return cudaStatus;
}
#endif

#if prac==2

int n;
#define BLOCK_SIZE 32
const int ELEM_PER_VECTOR = 32;
float (*pVecX)[ELEM_PER_VECTOR], (*pVecY)[ELEM_PER_VECTOR], (*pVecY_G)[ELEM_PER_VECTOR];
float(*pMatA)[ELEM_PER_VECTOR];
void init_MatVec(void)
{
	srand((unsigned)time(NULL));
	FILE* fp = fopen("gen.bin", "rb");
	fread(&n, sizeof(float), 1, fp);

	pVecX = new float[n][ELEM_PER_VECTOR];
	pVecY = new float[n][ELEM_PER_VECTOR];
	pVecY_G = new float[n][ELEM_PER_VECTOR];
	pMatA = new float[ELEM_PER_VECTOR][ELEM_PER_VECTOR];

	fread(pVecX, sizeof(float), n * ELEM_PER_VECTOR, fp);
	fread(pMatA, sizeof(float), ELEM_PER_VECTOR * ELEM_PER_VECTOR, fp);
	fclose(fp);
}
void Mat_Vec_Multiply()
{
	int i,j,k;
	float tmp = 0;
	for(i = 0; i < n; i++) {
		for(j = 0; j < ELEM_PER_VECTOR; j++) {
			tmp = 0;
			for(k = 0; k < ELEM_PER_VECTOR; k++) {
				tmp += pMatA[j][k] * pVecX[i][k];
			}
			pVecY[i][j] = tmp;
		}
	}
	// todo
}

//__global__ void Mat_Vec_Multiply_Kernel(const Array A, const Array B, Array C)
__global__ void Mat_Vec_Multiply_Kernel(float *A, float *B, float *C)
{ 
	int row = blockDim.y*blockIdx.y + threadIdx.y;
	int col = blockDim.x*blockIdx.x + threadIdx.x;
	int id = gridDim.x*blockDim.x*row + col;

	int i,j;
	float tmp=0;

	for(i = 0; i < ELEM_PER_VECTOR; i++) {
		tmp = 0;
		for(j = 0; j < ELEM_PER_VECTOR; j++) {
			tmp += A[i*ELEM_PER_VECTOR+j] * B[id*ELEM_PER_VECTOR+j];
		}
		C[id*ELEM_PER_VECTOR+i] = tmp;
	}

	// todo
}

//void Mat_Vec_Multiply_GPU(float *A, float *X, float *Y)
void Mat_Vec_Multiply_GPU()
{
	// Choose which GPU to run on, change this on a multi-GPU system.
	CUDA_CALL(cudaSetDevice(0));

	float *d_A, *d_X, *d_Y;
	size_t size;

	int n1, n2;

	n1 = BLOCK_SIZE;
	n2 = n/BLOCK_SIZE;

	size = ELEM_PER_VECTOR*ELEM_PER_VECTOR*sizeof(float);
	CUDA_CALL( cudaMalloc(&d_A, size) );
	CUDA_CALL( cudaMemcpy(d_A, pMatA, size, cudaMemcpyHostToDevice) );

	size = n*ELEM_PER_VECTOR*sizeof(float); 
	CUDA_CALL( cudaMalloc(&d_X, size) );
	CUDA_CALL( cudaMemcpy(d_X, pVecX, size, cudaMemcpyHostToDevice) );

	size = n*ELEM_PER_VECTOR*sizeof(float); 
	CUDA_CALL( cudaMalloc(&d_Y, size) );

	// Assume that width and height are multiples of BLOCK SIZE.
	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	dim3 dimGrid( n1/dimBlock.x, n2/dimBlock.y);
	
	CHECK_TIME_INIT_GPU()
	CHECK_TIME_START_GPU()
	Mat_Vec_Multiply_Kernel <<< dimGrid, dimBlock >>> (d_A, d_X, d_Y);
	CHECK_TIME_END_GPU(device_time)
	CHECK_TIME_DEST_GPU()

	// cudaDeviceSynchronize waits for the kernel to finish, and returns
	// any errors encountered during the launch.
	CUDA_CALL(cudaDeviceSynchronize())

	size = n*ELEM_PER_VECTOR*sizeof(float);
	CUDA_CALL( cudaMemcpy(pVecY_G, d_Y, size, cudaMemcpyDeviceToHost))
	
	
}

int main()
{
	init_MatVec();
	printf("n = %d  file open ok.\n", n);

	CHECK_TIME_START;
	Mat_Vec_Multiply();
	CHECK_TIME_END(compute_time);

	printf("***CPU C[10] = %.3f/ Time taken = %.6fms\n", pVecY[n-1][0], compute_time);
	Mat_Vec_Multiply_GPU();
	printf("***GPU C[10] = %.3f/ Time taken = %.6fms\n", pVecY_G[n-1][0], device_time);
	
}


#endif

#if prac==3

#define BLOCK_SIZE 16
#define N 67108864 // 8192 * 8192 = 2^13 * 2^13
int Fibonacci(int n) {
	// DO NOT MODIFY THIS FUNCTION!!!
	float sqrt_5, x_0, x_1;
	float tmp_0, tmp_1;

	sqrt_5 = sqrtf(5.0f);
	x_0 = (1.0f + sqrt_5) / 2.0f;
	x_1 = (1.0f - sqrt_5) / 2.0f;

	tmp_0 = tmp_1 = 1.0f;
	for (int i = 0; i < n; i++) {
		tmp_0 *= x_0;
		tmp_1 *= x_1;
	}
	return (int)((tmp_0 - tmp_1) / sqrt_5 + 0.5);
}
void generate_input(int *x, int n) {
	// DO NOT MODIFY THIS FUNCTION!!!
	srand((unsigned int)time(NULL));

	for (int i = 0; i < n; i++) {
		x[i] = 35 + (int)(5.0f * rand() / RAND_MAX + 0.5f);
	}
}
__global__ void Fibonacci_Kernel(int *x, int *y)
{
	int row = blockDim.y*blockIdx.y + threadIdx.y;
	int col = blockDim.x*blockIdx.x + threadIdx.x;
	int id = gridDim.x*blockDim.x*row + col;
	//int id = threadIdx.x;
	float sqrt_5, x_0, x_1;
	float tmp_0, tmp_1;

	sqrt_5 = sqrtf(5.0f);
	x_0 = (1.0f + sqrt_5) / 2.0f;
	x_1 = (1.0f - sqrt_5) / 2.0f;

	tmp_0 = tmp_1 = 1.0f;
	for (int i = 0; i < x[id]; i++) {
		tmp_0 *= x_0;
		tmp_1 *= x_1;
	}
	y[id] = (int)((tmp_0 - tmp_1) / sqrt_5 + 0.5);
}
void Fibonacci_GPU(int *x, int *y)
{
	cudaError_t cudaStatus;
		// Choose which GPU to run on, change this on a multi-GPU system.
		cudaStatus = cudaSetDevice(0);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
	}

	int *d_x, *d_y;
	size_t size;

	int n1, n2;

	n1 = 1024;
	n2 = N/1024;

	size = N*sizeof(int);
	CUDA_CALL( cudaMalloc(&d_x, size) );
	CUDA_CALL( cudaMemcpy(d_x, x, size, cudaMemcpyHostToDevice) );

	size = N*sizeof(int); 
	CUDA_CALL( cudaMalloc(&d_y, size) );
	
	// Assume that width and height are multiples of BLOCK SIZE.
	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	dim3 dimGrid( n1/dimBlock.x, n2/dimBlock.y);
	
	CHECK_TIME_INIT_GPU()
	CHECK_TIME_START_GPU()
	Fibonacci_Kernel <<< dimGrid, dimBlock >>> (d_x, d_y);
	CHECK_TIME_END_GPU(device_time)
	CHECK_TIME_DEST_GPU()

	// cudaDeviceSynchronize waits for the kernel to finish, and returns
	// any errors encountered during the launch.
	CUDA_CALL(cudaDeviceSynchronize())

	size = N*sizeof(int);
	CUDA_CALL( cudaMemcpy(y, d_y, size, cudaMemcpyDeviceToHost))
	

}

int *x, *y_c, *y_g; // input/output arrays

void main(void) {
	
	int n, i;
	FILE *fp;

	// Read the input array from the input file if one already exists.
	fp = fopen("x.binary", "rb");
	if (!fp) {
		fprintf(stderr, "Error: cannot open the input file...\n");
		exit(-1);
	}
	fread(&n, sizeof(int), 1, fp);
	fprintf(stdout, "\n*** The problem size is %d.\n", n);

	x = (int *)malloc(sizeof(int)*n);
	if (!x) {
		fprintf(stderr, "Error: cannot allocate memory for the input array...\n");
		exit(-1);
	}
	fread(x, sizeof(int), n, fp);
	fclose(fp);

	y_c = (int *)malloc(sizeof(int)*n); 
	y_g = (int *)malloc(sizeof(int)*n);

	//CPU
	CHECK_TIME_START;
	for (i = 0; i < n; i++) {
		y_c[i] = Fibonacci(x[i]);
	}
	CHECK_TIME_END(compute_time);
	//GPU
	Fibonacci_GPU(x, y_g);


	fprintf(stdout, "\n***_CPU_ Time taken for computing %d Fibonacci numbers is %.6fms\n\n", n, compute_time);
	fprintf(stdout, "\n***_GPU_ Time taken for computing %d Fibonacci numbers is %.6fms\n\n", n, device_time);
	//fprintf(stdout, "\n***_GPU_ Time taken for computing %d Fibonacci numbers is %.6fms\n\n", n, device_time);

	i = (int)(n * (rand() / (RAND_MAX + 1.0f)));
	fprintf(stdout, "*** Fibonacci number of %d is (CPU :%d  , GPU :%d).\n\n", x[i], y_c[i], y_g[i]);

	// Write the output array into the output file.
	fp = fopen("y.binary", "wb");
	if (!fp) {
		fprintf(stderr, "Error: cannot open the output file...\n");
		exit(-1);
	}
	fwrite(&n, sizeof(int), 1, fp);
	fwrite(y_c, sizeof(int), n, fp);
	fclose(fp);

	free(x);
	free(y_c);
	free(y_g);
}


#endif

#if prac==4

const int ELEM_PER_VECTOR = 32;

int main()
{
	int n;

	srand((unsigned)&n);

	printf("Enter a size: ");
	scanf("%d", &n);

	int size = ELEM_PER_VECTOR * n;
	float* vec = new float[size];

	for (int i = 0; i < size; ++i)
	{
		vec[i] = (float(rand()) * 2.f / RAND_MAX) - 1.f;
	}

	float(*mat)[ELEM_PER_VECTOR] = new float[ELEM_PER_VECTOR][ELEM_PER_VECTOR];
	for (int i = 0; i < ELEM_PER_VECTOR; ++i)
	{
		for (int j = 0; j < ELEM_PER_VECTOR; ++j)
		{
			mat[i][j] = (float(rand()) * 2.f / RAND_MAX) - 1.f;
		}
	}

	FILE* fp = fopen("gen.bin", "wb");
	fwrite(&n, sizeof(float), 1, fp);
	fwrite(vec, sizeof(float), size, fp);
	fwrite(mat, sizeof(float), ELEM_PER_VECTOR * ELEM_PER_VECTOR, fp);
	fclose(fp);

	fp = fopen("gen.bin", "rb");
	float* vec2 = new float[size];
	float(*mat2)[ELEM_PER_VECTOR] = new float[ELEM_PER_VECTOR][ELEM_PER_VECTOR];
	int m;
	fread(&m, sizeof(float), 1, fp);
	fread(vec2, sizeof(float), m * ELEM_PER_VECTOR, fp);
	fread(mat2, sizeof(float), ELEM_PER_VECTOR * ELEM_PER_VECTOR, fp);

	if (n != m) printf("error: size diff. %n != %n", n, m);
	for (int i = 0; i < size; ++i)
	{
		if (vec[i] != vec2[i])
		{
			printf("[%d] %f != %f\n", vec[i], vec2[i]);
			break;
		}
	}
	for (int i = 0; i < ELEM_PER_VECTOR; ++i)
	{
		for (int j = 0; j < ELEM_PER_VECTOR; ++j)
		{
			if (mat[i][j] != mat2[i][j])
			{
				printf("[%d][%d]\n", i, j);
				break;
			}
		}
	}
	fclose(fp);

	delete[] vec;

	return 0;
}

#endif