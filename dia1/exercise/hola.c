#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv)
{
    int myrank, nprocs,i;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	for(i=0;i<nprocs;i++){
		if(myrank == i)
			printf("Hola soy el procesador %d\n", myrank);
			MPI_Barrier(MPI_COMM_WORLD);
	}

    MPI_Finalize();
    return 0;
}
