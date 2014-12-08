/**
** Archivo: mpi.c
**/
#include <mpi.h>
#include <stdio.h>
#include <unistd.h>
int run_kernel(int dato, int rank);
int main(int argc, char *argv[])
{
	int rank, size, dato=0;
	char host[20];
	MPI_Status estado;
	MPI_Init (&argc, &argv); // Inicio de la comunicación entre procesos.
	MPI_Comm_size (MPI_COMM_WORLD, &size);// Se obtiene el número total de procesos.
	MPI_Comm_rank (MPI_COMM_WORLD, &rank); // Se obtiene el ID del proceso.
	// Todos Obtienen su hostname.
	gethostname(host, 20);
	if(rank > 0 && rank < (size-1))
	{
		MPI_Recv
		(
			&dato, // Referencia al vector donde se almacenara lo recibido
			1, // tamaño del vector a recibir
			MPI_INT, // Tipo de dato que recibe
			rank-1, // pid del proceso origen de la que se recibe
			0, // etiqueta
			MPI_COMM_WORLD, // Comunicador por el que se recibe
			&estado // estructura informativa del estado
		); 
		printf
		(
			"\nProceso[ %d ] desde %s : Recibi Dato = %d . Envio %d + %d \n",
			rank,
			host,
			dato,
			dato,
			rank
		);
		dato=run_kernel(dato,rank);
		MPI_Send(&dato, 1 ,MPI_INT ,rank+1 , 0 ,MPI_COMM_WORLD); 
	}else if(rank == size-1){
		MPI_Recv
		(
			&dato, // Referencia al vector donde se almacenara lo recibido
			1, // tamaño del vector a recibir
			MPI_INT, // Tipo de dato que recibe
			rank-1, // pid del proceso origen de la que se recibe
			0, // etiqueta
			MPI_COMM_WORLD, // Comunicador por el que se recibe
			&estado); // estructura informativa del estado
		printf
		(
			“\nProcess[ %d ] desde %s : Recibi Dato = %d . Envio %d + %d \n",
			rank,
			host,
			dato,
			dato,
			rank
		);
		
		dato=run_kernel(dato,rank);
		MPI_Send(&dato, 1 ,MPI_INT ,0 , 0 ,MPI_COMM_WORLD);
	}else if(rank == 0){ 
	printf
	(
	"\nProceso[ %d ] desde %s : Enviando Dato = %d \n",
	rank,
	host,
	dato
	);
	MPI_Send
	(
		&dato, //referencia al vector de elementos a enviar
		1, // tamaño del vector a enviar
		MPI_INT, // Tipo de dato que envias
		rank+1, // pid del proceso destino
		0, //etiqueta
		MPI_COMM_WORLD //Comunicador por el que se manda
	); 
	MPI_Recv
	(
		&dato, // Referencia al vector donde se almacenara lo recibido
		1, // tamaño del vector a recibir
		MPI_INT, // Tipo de dato que recibe
		size-1, // pid del proceso origen de la que se recibe
		0, // etiqueta
		MPI_COMM_WORLD, // Comunicador por el que se recibe
		&estado // estructura informativa del estado
	);
	printf
	(
		"\nProceso[ %d ] desde %s : Recibi Dato = %d \n\n",
		rank,
		host,
		dato
	);
}
	MPI_Finalize(); 
	return 0;
} 