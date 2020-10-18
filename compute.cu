// compute.cu
//
// driver and kernel call

#include <stdio.h>

#define THREADS_PER_BLOCK 256
 
__global__ void compute_d (int *a_d, int *b_d, int *c_d, int n)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    if (x < n) {
		a_d[x] = x % 10 + 1;

        if (x < (n / 2))
        {
            b_d[x] = x + 1;
            //j = x + 1;
        } else if (x == (n / 2) && 0 < n % 2)
        {
            b_d[x] = x + 1;
        } else
        {
            b_d[x] = (n - x);
        }
		c_d[x] = a_d[x] * b_d[x];
	}
		
}

extern "C" void computeArr (int *c, int arraySize)
{
	int *a_d, *b_d, *c_d;

	cudaMalloc ((void**) &a_d, sizeof(int) * arraySize);
	cudaMalloc ((void**) &b_d, sizeof(int) * arraySize);
	cudaMalloc ((void**) &c_d, sizeof(int) * arraySize);

	compute_d <<< ceil((float) arraySize/THREADS_PER_BLOCK), THREADS_PER_BLOCK >>> (a_d, b_d, c_d, arraySize);
	
	cudaError_t err = cudaGetLastError();
	if (err != cudaSuccess)
		printf ("CUDA error: %s\n", cudaGetErrorString(err));
		
	cudaMemcpy (c, c_d, sizeof(int) * arraySize, cudaMemcpyDeviceToHost);
	cudaFree (a_d);
	cudaFree (b_d);
	cudaFree (c_d);
}

