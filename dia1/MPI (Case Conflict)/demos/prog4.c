/*******************************************/
/*                                         */
/*             GPU 2014   - Ejemplo 4      */
/*                                         */
/*******************************************/

#include <stdio.h>
#include <math.h>
#include <mpi.h>

int main (int argc, char *argv[])
{
  int rank;
  MPI_Init (&argc, &argv);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  printf ("Hola, soy el PE (processing element) numero %d\n", rank);
  MPI_Finalize ();
}
