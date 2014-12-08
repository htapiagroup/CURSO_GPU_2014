#include <cuda.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#define STOP 0
#define START 1

/* Play with the following two values */
#define NB 1000000L //Size of array (long integer)
#define MANY 200L    //Number of transfers

/* (over-)Simple chronometer function */
void chrono (int kind, float *time) {
  static clock_t counts;
  if (kind == START) {
    *time = 0.0;
    counts = clock();
    return;
  }
  if (kind == STOP) {
    *time = ((float)(clock()-counts))/((float)CLOCKS_PER_SEC);
  }
}

int main () {
  float *ptr, *gpu_ptr;
  cudaError_t err;
  float time, number_of_Gbytes;
  long i, j;
  
  /* Dynamic allocations below */
  //  Allocate ptr on host below
    ptr = (float *)malloc(NB * sizeof(float));
  // Allocate gpu_ptr on device below
 err = cudaMalloc(&gpu_ptr,sizeof(float)*NB);

  /* Some error handling */
  if (ptr == NULL) {
    printf ("Not enough memory on host\n");
    exit (1);
  }
  if (err != 0) {
    printf  ("Pb allocating memory on device. Reason:\n");
    printf ("%s\n", cudaGetErrorString (err));
    exit (1);
  }

  /* Are the following two lines necessary ? */
  for (i = 0; i < NB; i++) 
    ptr[i] = 9.0;

  /* Transfer loop below */
  chrono (START, &time);
  for (j = 0; j < MANY; j++) {
    // perform memory transfer here
//    cudaMemcpy(gpu_ptr,ptr,sizeof(float)*NB,cudaMemcpyHostToDevice);
    cudaMemcpy(gpu_ptr,ptr,sizeof(float)*NB,cudaMemcpyDeviceToHost);
  }
  chrono (STOP, &time);

  /* Output results */
  number_of_Gbytes = (MANY*(float)sizeof(float)*NB/1024./1024./1024.);
  printf ("%f Gbytes transfered in %f seconds\n", number_of_Gbytes, time);
  printf ("Bandwidth = %f Gb/s\n", number_of_Gbytes / time);

  /* Clean the place */
  cudaFree (gpu_ptr);
  free (ptr);
  return 0;
}
  
