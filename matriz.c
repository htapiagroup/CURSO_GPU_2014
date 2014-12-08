#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE_X 8
#define SIZE_Y 8

float* matrix_alloc () {
  float *p;
  p = (float *)malloc(SIZE_X*SIZE_Y*sizeof(float));
  if (p == NULL) {
    fprintf (stderr, "I cannot reserve host memory for the matrix.\n");
    exit (EXIT_FAILURE);
  }
  return p;
}

//Inicializacion de la matriz en la cpu, con numeros aleatorios.
void initialize_matrix (float *m) {
  int i, j;
  for (i = 0; i < SIZE_X; i++) {
    for (j = 0; j < SIZE_Y; j++) {
      m[i+j*SIZE_X] = drand48();
    }
  }
}

void sum_matrices (float *res, float *a, float *b)  {
  int i,j,index;
  for (j = 0; j< SIZE_Y; j++) { 
    for (i = 0; i < SIZE_X; i++) {
      index = i+j*SIZE_X;
      res[index] = a[index] + b[index];
    }
  }
}

//Parte principal
int main () {
  float *matrix1, *matrix2, *result;
  matrix1 = matrix_alloc();
  result  = matrix_alloc();
  initialize_matrix (matrix1);
  initialize_matrix (matrix2);
  sum_matrices (result, matrix1, matrix2);
  printf ("%f",result);
}
