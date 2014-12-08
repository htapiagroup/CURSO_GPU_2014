#include <cuda.h>
 
/* Size of a block */
#define BLOCK_X 32
#define BLOCK_Y 16

__global__ void kernadd (float* mout, float* min1, float *min2, int nx, int ny, size_t pitch)
{
  int i, j, index;
  /* UP TO YOU edit line below so that the index is correctly evaluated */
  i = blockDim.x * blockIdx.x +threadIdx.x;
  j = blockDim.y * blockIdx.y +threadIdx.y;  

  index= i + j * pitch/sizeof(float);
  
  if ((i < nx) && (j < ny))
    mout[index] = min1[index] + min2[index];
}

/* extern "C" below is because this file follows C++ linking conventions */
/* whereas the companion C file (addition.c) follows C linking conventions */
/* which are different */

extern "C" void AddOnGpu(float* mat_out, float *mat_in1,  float *mat_in2,\
                         int nx, int ny) {

  size_t pitch;			/* Same pitch for all matrices, since
				   they all have same size */

  /* Matrix allocation on device */
  float *mat_out_gpu, *mat_in1_gpu, *mat_in2_gpu;
  /* UP TO YOU : do the allocation below, using cudaMallocPitch ()*/


  cudaMallocPitch(&mat_out_gpu, &pitch, sizeof(float)*nx, ny);
  cudaMallocPitch(&mat_in1_gpu, &pitch, sizeof(float)*nx, ny);
  cudaMallocPitch(&mat_in2_gpu, &pitch, sizeof(float)*nx, ny);
  /* The arguments mat_in1 and mat_in2 passed above are on the host. */
  /* UP TO YOU : write below the instructions to copy it to the device */
  /* You'll need to google the function cudaMemcpy2D () */
  
  cudaMemcpy2D(mat_in1_gpu, pitch, mat_in1, nx*sizeof(float), nx*sizeof(float), ny, cudaMemcpyHostToDevice);
  cudaMemcpy2D(mat_in2_gpu, pitch, mat_in2, nx*sizeof(float), nx*sizeof(float), ny, cudaMemcpyHostToDevice);
  

  /* Grid topology below */
  /* A block is BLOCK_X threads wide by BLOCK_Y threads high */
  dim3 block (BLOCK_X, BLOCK_Y);

  /* UP TO YOU : complete the number of blocks below */

  int n1 = (nx+BLOCK_X-1/BLOCK_X);
  int n2 = (nx+BLOCK_Y-1/BLOCK_Y);

  dim3 grid (n1,n2);

  /* UP TO YOU : kernel invocation */


  kernadd <<< grid, block >>> (mat_out_gpu,mat_in1_gpu, mat_in2_gpu, nx, ny, pitch);
   
  cudaThreadSynchronize();

  /* We now transfer back the matrix from the device to the host */
  /* UP TO YOU : write cudaMemcpy2D () instruction below */

  cudaMemcpy2D(mat_out, nx * sizeof(float), mat_out_gpu, pitch, nx * sizeof(float),ny,cudaMemcpyDeviceToHost);


  /* free memory */
  cudaFree(mat_out_gpu);
  cudaFree(mat_in1_gpu);
  cudaFree(mat_in2_gpu);
}
