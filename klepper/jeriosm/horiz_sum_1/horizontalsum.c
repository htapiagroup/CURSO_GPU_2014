#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/resource.h>

///Tamanyo de la matriz
#define SIZE_X 400
#define SIZE_Y 700

//Tamanyo de la pequenya ventana de visualizacion
#define WINDOW_X 5
#define WINDOW_Y 5

//Posicion de la pequenya ventana de visualizacion
#define WINDOW_X_OFFSET 30
#define WINDOW_Y_OFFSET 101

//Prototipos de la funciones usadas en el archivo .cu
double *matrix_alloc_gpu (int, int);
void H2D (double *, double *, int, int);
void D2H (double *, double *, int, int);
void main_gpu (double *, double *, int, int);

//Reservacion de la memoria en el host
double* matrix_alloc_cpu (int size_x, int size_y) {
  double *p;
  p = (double *)malloc(size_x*size_y*sizeof(double));
  if (p == NULL) {
    fprintf (stderr, "I cannot reserve host memory for the matrix.\n");
    exit (EXIT_FAILURE);
  }
  return p;
}

//Inicializacion de la matriz en la cpu, con numeros aleatorios.
void initialize_matrix (double *m) {
  int i,j;
  for (j = 0; j < SIZE_Y; j++) {
    for (i = 0; i < SIZE_X; i++) {
      m[i+j*SIZE_X] = drand48();
    }
  }
}

//Ejecucion de la suma horizontal en la CPU
void horizontal_sum_cpu (double *m, double *s)  {
  int i,j;
  srand48(1);
  for (j = 0; j< SIZE_Y; j++) {
    for (i = SIZE_X-1; i >= 0; i--) {
      if (i == SIZE_X-1)
	s[i+j*SIZE_X] = m[i+j*SIZE_X];
      else
	s[i+j*SIZE_X] = s[i+1+j*SIZE_X]+m[i+j*SIZE_X];
    }
  }
}

//Parte principal
int main () {
  double *a_cpu, *s_cpu, *s2_cpu;
  double *a_gpu, *s_gpu, max=-1.0, m;
  int i, j;
  struct tms buffer;

  a_cpu = matrix_alloc_cpu (SIZE_X,SIZE_Y);
  a_gpu = matrix_alloc_gpu (SIZE_X,SIZE_Y);
  s_cpu = matrix_alloc_cpu (SIZE_X,SIZE_Y);
  s2_cpu = matrix_alloc_cpu (SIZE_X,SIZE_Y);
  s_gpu = matrix_alloc_gpu (SIZE_X,SIZE_Y);
  initialize_matrix (a_cpu);
  
  horizontal_sum_cpu (a_cpu, s_cpu);

  H2D (a_gpu, a_cpu, SIZE_X, SIZE_Y);
  main_gpu (s_gpu, a_gpu, SIZE_X, SIZE_Y);
  //AQUI: Falta una instruccion. Encuentre cual.

  for (j = WINDOW_Y_OFFSET; j < WINDOW_Y_OFFSET+WINDOW_Y; j++) {
    for (i = WINDOW_X_OFFSET; i < WINDOW_X_OFFSET+WINDOW_X; i++) {
      printf ("%.6g\t", s_cpu[i+j*SIZE_X]);
    }
    printf ("\n");
  }
  printf ("\n----\n\n");
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
  printf ("Maximum difference = %g\n", max);
  if (max < 1e-11) 
    printf ("Test passed\n");
  else
    printf ("Test failed\n");
}
