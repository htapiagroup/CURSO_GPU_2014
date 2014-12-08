#include <cuda.h>
#include <time.h>
/* Size of a block */
#define BLOCK_X 32
#define BLOCK_Y 16
#define MANY 200
#define STOP 0
#define START 1

__global__ void kernadd (float* mout, float* min1, float *min2, int nx, int ny, size_t pitch)
{
  int i, j, index;
  /* UP TO YOU edit line below so that the index is correctly evaluated */
  i = blockDim.x * blockIdx.x + threadIdx.x;
  j = blockDim.y * blockIdx.y + threadIdx.y;
  index = i + j * pitch / sizeof(float);
  if ((i < nx) && (j < ny))
    mout[index] = min1[index] + min2[index];
}

/* extern "C" below is because this file follows C++ linking conventions */
/* whereas the companion C file (addition.c) follows C linking conventions */
/* which are different */

extern "C" void chrono (int kind, float *time);

extern "C" float AddOnGpu(float* mat_out, float *mat_in1,  float *mat_in2,int nx, int ny) {

  size_t pitch;			/* Same pitch for all matrices, since
				   they all have same size */

  /* Matrix allocation on device */
  float *mat_out_gpu, *mat_in1_gpu, *mat_in2_gpu;
  /* UP TO YOU : do the allocation below, using cudaMallocPitch ()*/
  cudaMallocPitch (&mat_out_gpu, &pitch, sizeof(float) * nx, ny);
  cudaMallocPitch (&mat_in1_gpu, &pitch, sizeof(float) * nx, ny);
  cudaMallocPitch (&mat_in2_gpu, &pitch, sizeof(float) * nx, ny);
  /* The arguments mat_in1 and mat_in2 passed above are on the host. */
  /* UP TO YOU : write below the instructions to copy it to the device */
  /* You'll need to google the function cudaMemcpy2D () */
  cudaMemcpy2D(mat_in1_gpu,pitch,mat_in1,nx*sizeof(float),nx*sizeof(float),ny,cudaMemcpyDeviceToHost);
  cudaMemcpy2D(mat_in2_gpu,pitch,mat_in2,nx*sizeof(float),nx*sizeof(float),ny,cudaMemcpyDeviceToHost);
  /* Grid topology below */
  /* A block is BLOCK_X threads wide by BLOCK_Y threads high */
  dim3 block (BLOCK_X, BLOCK_Y);

  /* UP TO YOU : complete the number of blocks below */

  int n1 = (nx+BLOCK_X-1)/BLOCK_X;
  int n2 = (nx+BLOCK_Y-1)/BLOCK_Y;;

  dim3 grid (n1,n2);
  
  int count;
  float time;
  chrono (START, &time);
  /* UP TO YOU : kernel invocation */
  for (count = 0;count < MANY; count++){
    kernadd <<< grid,block >>> (mat_out_gpu, mat_in1_gpu,mat_in2_gpu,nx,ny,pitch);
    cudaThreadSynchronize();
  }
  chrono (STOP, &time);

  /* We now transfer back the matrix from the device to the host */
  /* UP TO YOU : write cudaMemcpy2D () instruction below */
  
  cudaMemcpy2D (mat_out,sizeof(float)*nx,mat_out_gpu,pitch,nx*sizeof(float),ny,cudaMemcpyDeviceToHost);

  /* free memory */
  cudaFree(mat_out_gpu);
  cudaFree(mat_in1_gpu);
  cudaFree(mat_in2_gpu);
  return time/float(MANY);
}
