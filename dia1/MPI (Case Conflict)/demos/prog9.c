/*******************************************/
/*                                         */
/*             GPU 2014   - Ejemplo 9      */
/*                                         */
/*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

static int *buffer;
static long NX, NY;
static int rank, size;

int *Init (long nx, long ny)
{
  long i, j, l;
  long c1, c2;
  int *array;
  array  = malloc(sizeof(int)*(nx+2)*(ny+2));
  buffer = malloc(sizeof(int)*(nx+2)*(ny+2));
  c1 = nx/2;
  c2 = ny/2;
  for (i = 0; i <= nx+1; i++) {
    for (j = 0; j <= ny+1; j++) {
      l = i*(ny+2)+j;
      array[l] = 0;
      // Dibujo de la "U" inicial
      if ((i == c1) && (j == c2)) array[l] = 1;
      if ((i == c1-1) && (j == c2)) array[l] = 1;
      if ((i == c1+1) && (j == c2)) array[l] = 1;
      if ((i == c1-1) && (j == c2-1)) array[l] = 1;
      if ((i == c1-1) && (j == c2-2)) array[l] = 1;
      if ((i == c1+1) && (j == c2-1)) array[l] = 1;
      if ((i == c1+1) && (j == c2-2)) array[l] = 1;
    }
  }
  return array;
}

void Lagartijas (long nb)
{
  long i;
  double x=1e20;
  for (i = 0; i < nb; i++) {
    x = sqrt(1.+x);
  }
}

void Itere (int *array, long nx, long ny)
{
  long i, j, k, l, lv, m;
  long n_vecinos;
  for (i = 1; i <= nx; i++) {
    for (j = 1; j <= ny; j++) {
      m = i*(ny+2)+j;
      n_vecinos = 0;
      for (k=-1; k <=1; k++) {
	for (l=-1; l <=1; l++) {
	  lv = l+k*(ny+2);
	  if ((l != 0) || (k != 0))
	    n_vecinos = n_vecinos + array[m+lv];
	}
      }
      buffer[m] = 0;
      if (n_vecinos == 3) buffer[m] = 1;
      if (n_vecinos == 2) buffer[m] = array[m];
    }
  }
  for (i = 1; i <= nx; i++) {
    for (j = 1; j <= ny; j++) {
      m = i*(ny+2)+j;
      array[m] = buffer[m];
      Lagartijas (1000L);
    }
  }
}

void Output (int *array, long nx, long ny)
{
  long ii, jj, l=0;
  FILE *out;
  for (ii = 1; ii <= nx; ii++) {
    for (jj = 1; jj <= ny; jj++) {
      buffer[l] = array[jj+ii*(ny+2)];
      l = l+1;
    }
  }
  out = fopen ("out0", "w");
  if (out == NULL) {
    printf ("Error\n");
    exit (1);
  }
  fwrite (buffer, sizeof(int), nx*ny, out);
  fclose (out);
}

int main (int argc, char *argv[])
{
  int *array;
  long nx, ny;
  int i;
  NX = 100; NY = 100;
  nx = NX; ny = NY;
  array = Init (nx, ny);
  for (i = 0; i < 111; i++) {
    Output (array, nx, ny);
    Itere (array, nx, ny);
  }
  free (array);
  free (buffer);
}
