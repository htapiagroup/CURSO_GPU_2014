/*******************************************/
/*                                         */
/*             GPU 2014   - Ejemplo 3      */
/*                                         */
/*******************************************/

#include <stdio.h>
#include <math.h>
#include <mpi.h>

int main (int argc, char *argv[])
{
  printf ("Hello world\n");
  MPI_Init (&argc, &argv);
  printf ("Loop infinito...\n");
  while (1) {};
  MPI_Finalize ();
}
