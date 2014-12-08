#include <cuda.h>
#include <stdio.h>

#define BLOCKSIZE 256

__global__ void kern_set_val (float *gpu_ptr, float value, int nb) {
  int i;
  i=blockDim.x * blockIdx.x+threadIdx.x;  
 //gpu_ptr[i] = value;
 //  gpu_ptr[i] = blockDim.x;
gpu_ptr[i] = blockIdx.x;
}

extern "C" void set_value (float *ptr, float value, int nb) {
  float *gpu_ptr;
  cudaMalloc (&gpu_ptr, sizeof(float)*nb);

  //UP TO YOU : write kernel invocation here
  kern_set_val <<<nb/BLOCKSIZE,BLOCKSIZE>>>(gpu_ptr,value,nb);

  cudaThreadSynchronize ();

  cudaMemcpy(ptr,gpu_ptr,nb*sizeof(float),cudaMemcpyDeviceToHost);

  cudaFree (gpu_ptr);
}
