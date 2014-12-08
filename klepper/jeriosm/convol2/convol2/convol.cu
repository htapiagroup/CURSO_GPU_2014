#include <time.h>
#include <cuda.h>
#include <stdio.h>
#define STOP 0
#define START 1

#define BLOCK_X 16
#define BLOCK_Y 16

extern "C" void chrono (int kind, float *time);

__global__ void kconvol (float *gpu_a, float *gpu_b, int pitch, int n) {
  int ig, jg, lg, il, jl, ll;
  __shared__ float la[(BLOCK_X+2)*(BLOCK_Y+2)];
  __shared__ float lb[(BLOCK_X+2)*(BLOCK_Y+2)];
 
 // A thread now has two sets of coordinates : 
  // (ig, jg) in the global array
  // (il, jl) in the local array (shared) of size (BLOCK_X+2)*(BLOCK_Y+2)
  ig = blockDim.x*blockIdx.x+threadIdx.x;
  jg = blockDim.y*blockIdx.y+threadIdx.y;
  lg = ig+jg*pitch;
  // UP TO YOU : write below the indices il and jl
  il = threadIdx.x+1;
  jl = threadIdx.y+1;
  ll = il+jl*(BLOCK_X+2);
 
 // What does the following line correspond to ?
  la[ll] = gpu_a[lg];
  if ((il == 1) && (ig > 0)) // What does the following line correespond to ?
    la[ll-1] = gpu_a[lg-1];
  if ((jl == 1) && (jg > 0))
    la[ll-BLOCK_X-2] = gpu_a[lg-pitch];
  if ((il == BLOCK_X) && (ig < n-1)) // UP TO YOU The following line is missing. Find out what was intended
    la[ll-BLOCK_X-2]= gpu_a[lg+pitch];
  if ((jl == BLOCK_Y) && (jg < n-1)) // UP TO YOU Find out the missing offset of local array below
    la[ll+BLOCK_X+2] = gpu_a[lg+pitch];
  __syncthreads ();
  if ((ig >= n) || (jg >= n)) return;
  if ((ig == 0) || (jg == 0) || (ig == n-1) || (jg == n-1))  {
    lb[ll] = la[ll];
  }
  else
    /* UP TO YOU : fill up below the missing indices */
    lb[ll]=(1.f/5.f)*(                +la[ll-BLOCK_X-2]+	     \
		      la[ll-1]        +la[ll]             +la[ll+1]+ \
		                      +la[ll+BLOCK_X+2]);
if((ig<n) &&(ig>0))  
gpu_b[lg] = lb[ll];
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
  grid.x = (n-1)/BLOCK_X+1;
  grid.y = (n-1)/BLOCK_Y+1;
  cudaMemcpy2D (gpu_a, pitch, a, n*sizeof(float), n*sizeof(float), n, cudaMemcpyHostToDevice);
  chrono (START, &time);
  kconvol <<<grid, block>>> (gpu_a, gpu_b, pitch/sizeof(float), n);
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
