#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define SIZE_X 800
#define SIZE_Y 2000

#define WINDOW_X 5
#define WINDOW_Y 5

#define WINDOW_X_OFFSET 30
#define WINDOW_Y_OFFSET 101

float *matrix_alloc_gpu (int, int);
void H2D (float *, float *, int, int);
void D2H (float *, float *, int, int);
void main_gpu (float *, float *, int, int);


float* matrix_alloc_cpu (int size_x, int size_y) {
  float *p;
  p = (float *)malloc(size_x*size_y*sizeof(float));
  if (p == NULL) {
    fprintf (stderr, "I cannot reserve host memory for the matrix.\n");
    exit (EXIT_FAILURE);
  }
  return p;
}

void initialize_matrix (float *m) {
  int i,j;
  srand48(1);
  for (j = 0; j < SIZE_Y; j++) {
    for (i = 0; i < SIZE_X; i++) {
      m[i+j*SIZE_X] = drand48();
    }
  }
}

void vertical_sum_cpu (float *m, float *s)  {
  int i,j;
  for (j = 0; j< SIZE_Y; j++) {
    for (i = 0; i < SIZE_X; i++) {
      if (j == 0)
	s[i+j*SIZE_X] = 0.0;
      else
	s[i+j*SIZE_X] = s[i+(j-1)*SIZE_X]+m[i+(j-1)*SIZE_X];
    }
  }
}

int main () {
  float *a_cpu, *s_cpu, *s2_cpu;
  float *a_gpu, *s_gpu, max=-1.0, m;
  int i, j;

  a_cpu = matrix_alloc_cpu (SIZE_X,SIZE_Y);
  a_gpu = matrix_alloc_gpu (SIZE_X,SIZE_Y);
  s_cpu = matrix_alloc_cpu (SIZE_X,SIZE_Y);
  s2_cpu = matrix_alloc_cpu (SIZE_X,SIZE_Y);
  s_gpu = matrix_alloc_gpu (SIZE_X,SIZE_Y);
  initialize_matrix (a_cpu);
  
  vertical_sum_cpu (a_cpu, s_cpu);

  H2D (a_gpu, a_cpu, SIZE_X, SIZE_Y);
  main_gpu (s_gpu, a_gpu, SIZE_X, SIZE_Y);
  for (j = WINDOW_Y_OFFSET; j < WINDOW_Y_OFFSET+WINDOW_Y; j++) {
    for (i = WINDOW_X_OFFSET; i < WINDOW_X_OFFSET+WINDOW_X; i++) {
      printf ("%.6g\t", s_cpu[i+j*SIZE_X]);
    }
    printf ("\n");
  }
  printf ("\n----\n\n");

  D2H (s2_cpu, s_gpu, SIZE_X, SIZE_Y);

  for (j = WINDOW_Y_OFFSET; j < WINDOW_Y_OFFSET+WINDOW_Y; j++) {
    for (i = WINDOW_X_OFFSET; i < WINDOW_X_OFFSET+WINDOW_X; i++) {
      printf ("%.6g\t", s2_cpu[i+j*SIZE_X]);
    }
    printf ("\n");
  }
  for (j = 0; j < SIZE_Y; j++) {
    for (i = 0; i < SIZE_X; i++) {
      m = fabs(s_cpu[i+j*SIZE_X]-s2_cpu[i+j*SIZE_X]);
      if (m > max) max = m;
    }
  }
}
