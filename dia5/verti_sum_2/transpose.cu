#include <stdio.h>
#include <stdlib.h>

#define TILE 16

__global__ void transposeNoBankConflicts(float *odata, float *idata,\
					 int in_pitch, int out_pitch) {
  __shared__ float tile[TILE][TILE+1];
  
  int xIndex = blockIdx.x * TILE + threadIdx.x;
  int yIndex = blockIdx.y * TILE + threadIdx.y;
  int index_in = xIndex + (yIndex)*in_pitch;

  // AQUI: Decomentar o bien la parte 1, o bien la parte 2
  
  //  PARTE 1
  //  xIndex = blockIdx.y * TILE + threadIdx.x;
  //  yIndex = blockIdx.x * TILE + threadIdx.y;
  //////////
  
  // PARTE 2
  //  xIndex = blockIdx.y * TILE + threadIdx.y;
  //  yIndex = blockIdx.x * TILE + threadIdx.x;
  //////////

  int index_out = xIndex + (yIndex)*out_pitch;
  
  tile[threadIdx.y][threadIdx.x] = idata[index_in];
  
  __syncthreads();

  // AQUI: Decomentar o bien la parte 1, o bien la parte 2
  // OJO: la parte que se tiene de decomentar depende de su eleccion mas arriba.
  // Cual combinacion es preferible ?

  // PARTE 1
  //  odata[index_out] = tile[threadIdx.x][threadIdx.y];
  //////

  // PARTE 2
  //  odata[index_out] = tile[threadIdx.y][threadIdx.x];
  //////
}

extern "C" void transpose (float *matrix_in, float *matrix_out, int inp, int outp) {
  dim3 grid, threads;

  if (inp % TILE != 0) {
    fprintf (stderr, "Size problem...\n");
    exit (EXIT_FAILURE);
  }
  if (outp % TILE != 0) {
    fprintf (stderr, "Size problem...\n");
    exit (EXIT_FAILURE);
  }
  grid.x = inp/TILE;
  grid.y = outp/TILE;
  threads.x = TILE;
  threads.y = TILE;
  transposeNoBankConflicts<<<grid,threads>>>(matrix_out,matrix_in,inp,outp);
}
