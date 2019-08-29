#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include <omp.h>

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	
	int my_rank = -1;
	int process_count = -1; //Cant de procesos que se van a distribuir
	
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); //rank = process id
	MPI_Comm_size(MPI_COMM_WORLD, &process_count);
	
	char hostname[MPI_MAX_PROCESSOR_NAME];
	int hostname_length = -1;
	MPI_Get_processor_name(hostname, &hostname_length);
	
	int papa = 0;
	
	if(argc >= 2)
	{
		papa = strtoll(argv[1], NULL, 10);
	}
	else
	{
		return (void)fprintf(stderr, "error: invalid input\n"), EXIT_SUCCESS;		
	}
	
	int next_process = (my_rank + 1) % process_count;
	int prev_process = my_rank - 1;

	if (my_rank == 0)
	{
		prev_process = process_count - 1;
		MPI_Send(&papa, 1, MPI_INT, next_process, my_rank, MPI_COMM_WORLD);
	}
		
	while( papa > 0 )
	{
		MPI_Recv(&papa, 1, MPI_INT, prev_process, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		--papa;
		if(papa == 0)
			printf("Potato exploted on process %d\n", my_rank);
			
		MPI_Send(&papa, 1, MPI_INT, next_process, my_rank, MPI_COMM_WORLD);
	}
	
	MPI_Finalize();
	return 0;
}
