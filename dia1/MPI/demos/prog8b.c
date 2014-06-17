/*******************************************/
/*                                         */
/*             GPU 2014   - Ejemplo 8b     */
/*                                         */
/*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main (int argc, char *argv[])
{
  double x, y, approx_pi, pi_pe, pi_medio;
  long n_dentro=0, n_total=40000000L, i;
  int rank, size;
  MPI_Init (&argc, &argv);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  MPI_Comm_size (MPI_COMM_WORLD, &size);
  srand48(rank);
  for (i=0L; i < n_total; i++) {
    x = drand48();
    y = drand48();
    if (x*x+y*y < 1.0) n_dentro++;
  }
  pi_pe=(double)n_dentro/(double)n_total*4.0;
  MPI_Allreduce (&pi_pe, &pi_medio, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  pi_medio = pi_medio/(double)size;
  if (rank == 0) printf ("Valor promedio del estimo de PI: %g\n", pi_medio);
  MPI_Finalize ();
}
