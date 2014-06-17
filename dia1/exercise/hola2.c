#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv){
	int rank,size,i,posta=1,msg;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Status status;
	
	if(rank == 0){
		MPI_Send(&posta,1,MPI_INT,rank+1,10,MPI_COMM_WORLD);
		printf("Soy el proceso %d\n",rank);
	}else if(rank < (size-1)){
		MPI_Recv(&msg,1,MPI_INT,rank-1,10,MPI_COMM_WORLD,&status);
		MPI_Send(&posta,1,MPI_INT,rank+1,10,MPI_COMM_WORLD);
		printf("Soy el proceso %d\n",rank);
	}
	else{
		MPI_Recv(&msg,1,MPI_INT,rank-1,10,MPI_COMM_WORLD,&status);
		printf("Soy el proceso %d\n",rank);
	}
	MPI_Finalize();
	return 0;
}