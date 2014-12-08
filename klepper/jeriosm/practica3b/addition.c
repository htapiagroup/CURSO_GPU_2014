// Note : LINUX users, uncomment the next line
// #include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


/* Define the matrix size */
#define NX 200
#define NY 100

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

  for (i=0; i< nx*ny; i++) {	/* No need for a 2D loop, actually ! */
    diff = mat_out[i] - (mat_in1[i]+mat_in2[i]);
    if (fabs(diff) > 0.0000001f) {
      error = 1;
    }
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
