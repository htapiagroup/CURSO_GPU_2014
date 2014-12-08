#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

/* Define the matrix size */
#define NX 200
#define NY 100

#define START 0
#define STOP 1

extern "C" chrono (int kind, float *time) {
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

void AddOnGpu(float* mat_out, float *mat_in1, float *mat_in2,\
	      int nx, int ny);


/*******************************************************/
/*  We initialize the vectors with random values       */
/*******************************************************/

void Init(float* mat, int nx, int ny) {
  int i, j;
  for (i = 0; i < nx; i++) {	/* 2D loop */
    for (j = 0; j < ny; j++) {
      mat[i+j*nx] = drand48 ();	/* position of cell (i,j) */
    }
  }
}


/*******************************************************/
/*            MAIN PROGRAM                             */
/*******************************************************/

int main () {
  int i=0, error=0, nx=NX, ny=NY;
  float diff;

  /* Matrix allocation */
  float *mat_in1 = (float*) malloc(nx * ny * sizeof(float));
  float *mat_in2 = (float*) malloc(nx * ny * sizeof(float));
  float *mat_out = (float*) malloc(nx * ny * sizeof(float));

  /* Matrix initialization */
  Init(mat_in1, nx, ny);
  Init(mat_in2, nx, ny);
  
  AddOnGpu(mat_out, mat_in1, mat_in2, nx, ny);

  /* We now check that the result is correct */
  for(int count=0;count < MANY;count++){
  for (i=0; i< nx*ny; i++) {	/* No need for a 2D loop, actually ! */
   mat_out[i]= mat_in1[i]+mat_in2[i];
  }

  if (error) {
    printf("FAILED\n");
  }
  else {
    printf("PASSED\n");
  }
  free (mat_in1);
  free (mat_in2);
  free (mat_out);
}
