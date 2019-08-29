#include <stdio.h>
#include <mpi.h>

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
	
	printf("Hello from main thread of process %d of %d on %s\n", my_rank, process_count, hostname);
	
	MPI_Finalize();
	return 0;
}
