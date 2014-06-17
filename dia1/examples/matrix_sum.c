#include <stdio.h>
#include <stdlib.h>
#include <math.h>

///Tamanyo de la matriz
#define SIZE_X 8000
#define SIZE_Y 7000

//Reservacion de la memoria
double* matrix_alloc () {
  double *p;
  p = (double *)malloc(SIZE_X*SIZE_Y*sizeof(double));
  if (p == NULL) {
    fprintf (stderr, "I cannot reserve host memory for the matrix.\n");
    exit (EXIT_FAILURE);
  }
  return p;
}

//Inicializacion de la matriz en la cpu, con numeros aleatorios.
void initialize_matrix (double *m) {
  int i, j;
  for (i = 0; i < SIZE_X; i++) {
    for (j = 0; j < SIZE_Y; j++) {
      m[i+j*SIZE_X] = drand48();
    }
  }
}

//Ejecucion de la suma horizontal en la CPU
void sum_matrices (double *res, double *a, double *b)  {
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
  double *matrix1, *matrix2, *result;
  matrix1 = matrix_alloc();
  matrix2 = matrix_alloc();
  result  = matrix_alloc();
  initialize_matrix (matrix1);
  initialize_matrix (matrix2);
  sum_matrices (result, matrix1, matrix2);
}
