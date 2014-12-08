// Note : LINUX users, uncomment the next line
// #include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#define STOP 0
#define START 1
#define MANY 200

/* Define the matrix size */
#define NX 2048
#define NY 1024

float AddOnGpu(float* mat_out, float *mat_in1, float *mat_in2,\
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




/*******************************************************/
/*            MAIN PROGRAM                             */
/*******************************************************/

int main () {
  int i=0, error=0, nx=NX, ny=NY, count;
  float diff,t_cpu,t_gpu,time;

  /* Matrix allocation */
  float *mat_in1 = (floa,t*) malloc(nx * ny * sizeof(float));
  float *mat_in2 = (float*) malloc(nx * ny * sizeof(float));
  float *mat_out = (float*) malloc(nx * ny * sizeof(float));

  /* Matrix initialization */
  Init(mat_in1, nx, ny);
  Init(mat_in2, nx, ny);
  
  t_gpu = AddOnGpu(mat_out, mat_in1, mat_in2, nx, ny);

  /* We now check that the result is correct */

  chrono (START, &time);
  for (count = 0;count < MANY; count++){
    for (i=0; i< nx*ny; i++) {  /* No need for a 2D loop, actually ! */
      mat_out[i] = mat_in1[i] + mat_in2[i];
    }
  }
  chrono (STOP, &time);
  
  t_cpu=time/(float)MANY;
  printf("Tiempo de CPU: %f segundos\n",t_cpu);
  printf("Tiempo de GPU: %f segundos\n",t_gpu);
  printf("factor de aceleracion %f segundos\n",t_cpu/t_gpu);
 
  free (mat_in1);
  free (mat_in2);
  free (mat_out);
}
