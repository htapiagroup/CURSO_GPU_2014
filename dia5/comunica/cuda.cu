/**** Archivo: cuda.cu **/
#include <stdio.h>
#include <unistd.h>
#include <cuda.h>
__global__ void kernel(int dato,int rank, int *gpu_dato){
	*gpu_dato= dato + rank;
}
extern "C"
int run_kernel(int dato, int rank)
{
int *gpu_dato;
cudaMalloc( (void**)&gpu_dato, sizeof(int));
kernel<<<1,1>>>(dato, rank, gpu_dato);
cudaMemcpy(&dato, gpu_dato, sizeof(int), cudaMemcpyDeviceToHost); 
return dato;
}