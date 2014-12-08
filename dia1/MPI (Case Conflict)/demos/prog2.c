/*******************************************/
/*                                         */
/*             GPU 2014   - Ejemplo 2      */
/*                                         */
/*******************************************/

#include <stdio.h>
#include <math.h>
#include <mpi.h>

int main (int argc, char *argv[])
{
  MPI_Init (&argc, &argv);
  printf ("Hello world\n");
  MPI_Finalize ();
}
