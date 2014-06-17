/*******************************************/
/*                                         */
/*             GPU 2014   - Ejemplo 7      */
/*                                         */
/*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main (int argc, char *argv[])
{
  double x, y, approx_pi;
  long n_dentro=0, n_total=10000L, i;
  int rank;
  MPI_Init (&argc, &argv);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  srand48(rank);
  for (i=0L; i < n_total; i++) {
    x = drand48();
    y = drand48();
    if (x*x+y*y < 1.0) n_dentro++;
  }
  printf ("Valor aproximado de PI: %g\n", (double)n_dentro/(double)n_total*4.0);
  MPI_Finalize ();
}
