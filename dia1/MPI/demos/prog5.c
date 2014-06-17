/*******************************************/
/*                                         */
/*             GPU 2014   - Ejemplo 5      */
/*                                         */
/*******************************************/

#include <stdio.h>
#include <math.h>
#include <mpi.h>

int main (int argc, char *argv[])
{
  int rank, size;
  MPI_Init (&argc, &argv);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  printf ("Hola, soy el PE (processing element) numero %d de %d en total\n", rank, size);
  MPI_Finalize ();
}
