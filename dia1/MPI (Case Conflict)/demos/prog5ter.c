/*******************************************/
/*                                         */
/*             GPU 2014   - Ejemplo 5ter   */
/*                                         */
/*******************************************/

#include <stdio.h>
#include <math.h>
#include <mpi.h>

int main (int argc, char *argv[])
{
  int rank, size, foo;
  MPI_Init (&argc, &argv);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (rank < size-1) MPI_Recv (&foo, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  fprintf (stderr, "Hola, soy el PE (processing element) numero %d\n", rank);
  fflush (stderr);
  if (rank > 0) MPI_Send (&foo, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD);
  MPI_Finalize ();
}
