#include <stdio.h>
#include <mpi.h>
#include <string.h>

int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);
	
	//MPI_Comm_rank(MPI_Comm comunicador, int *rank(id));
	int my_rank = -1;
	int process_count = -1; //Cant de procesos que se van a distribuir
	
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &process_count);
	
	char hostname[MPI_MAX_PROCESSOR_NAME];
	int hostname_length = -1;
	MPI_Get_processor_name(hostname, &hostname_length);
	
	char message[2048];//se crea el buffer de bytes para enviar la cadena y no sea memoria descontinua 
	if (my_rank != 0)	
	{
		sprintf(message, "Hello from main thread of process %d of %d on %s\n"
			, my_rank, process_count, hostname);
		MPI_Send(message, strlen(message) + 1, MPI_CHAR, 0/*destino*/, 0/*tag*/, MPI_COMM_WORLD);
			   //buffer, tamaño buffer, tipo de dato, proceso que recibe, "identificar envios",comunicador
	}
	else
		for ( int source = 1; source < process_count; ++source){
			MPI_Recv(message, 2048, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("[%d] %s", source, message);
		}
	
	MPI_Finalize();
	return 0;
}
