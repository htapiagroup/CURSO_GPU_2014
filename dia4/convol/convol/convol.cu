#include <time.h>
#include <cuda.h>
#include <stdio.h>
#define STOP 0
#define START 1

#define BLOCK_X 16
#define BLOCK_Y 16

extern "C" void chrono (int kind, float *time);

__global__ void kconvol (float *gpu_a, float *gpu_b, int pitch, int n) {
  int i, j, l;
  // UP TO YOU : evaluate the coordinate i of the current thread,
  // using blockDim, blockIdx and threadIdx.

  j = blockDim.y*blockIdx.y+threadIdx.y;
  // UP TO YOU : evaluate global 1D index of thread (i,j)
  // in array gpu_a, as a linear combination of i and j.
  // You will have to use the variable 'pitch'

  if ((i >= n) || (j >= n)) return;
  if ((i == 0) || (j == 0) || (i == n-1) || (j == n-1))  {
    gpu_b[l] = gpu_a[l]; // edges are untouched
  }
  else
    // UP TO YOU : fill up the MISSING indices below
    gpu_b[l]=(1.f/5.f)*(                 gpu_a[l-pitch]+\
			gpu_a[/*MISSING*/]  +gpu_a[l]         +gpu_a[l+1]+ \
			                 gpu_a[/*MISSING*/]                 );
}

extern "C" void gpu_convol (float *a, float *b, int n) {
  float *gpu_a;
  float *gpu_b;
  cudaError_t err;
  size_t pitch;
  float time;
  err = cudaMallocPitch (&gpu_a, &pitch, n*sizeof(float), n);
  if (err != 0) {
    printf ("Error allocating gpu_a: %s\n", cudaGetErrorString (err));
    exit (1);
  }
  err = cudaMallocPitch (&gpu_b, &pitch, n*sizeof(float), n);
  if (err != 0) {
    printf ("Error allocating gpu_b: %s\n", cudaGetErrorString (err));
    exit (1);
  }
  dim3 block (BLOCK_X, BLOCK_Y);
  dim3 grid;
  // UP TO YOU : how many blocks to we want in x (grid.x) and in y (grid.y) ?
  grid.x = ;
  grid.y = ;
  cudaMemcpy2D (gpu_a, pitch, a, n*sizeof(float), n*sizeof(float), n, cudaMemcpyHostToDevice);
  // NOTE : the chronometer below does not contemplate overhead of memory allocation and
  // memory transfer.
  chrono (START, &time);
  // UP TO YOU : the third variable is missing below in the kernel invocation
  // It is a pitch. Beware ! Does it represent a number of bytes, or a number of floats ?
  kconvol <<<grid, block>>> (gpu_a, gpu_b, /* MISSING */, n);
  err=cudaThreadSynchronize ();
  chrono (STOP, &time);
  printf ("Convolution took  %f sec. on GPU\n", time);
  cudaMemcpy2D (b, n*sizeof(float), gpu_b, pitch, n*sizeof(float), n, cudaMemcpyDeviceToHost);
  if (err != 0) {
    printf ("%s\n", cudaGetErrorString (err));
    exit (1);
  }
  cudaFree (gpu_a);
  cudaFree (gpu_b);
}
