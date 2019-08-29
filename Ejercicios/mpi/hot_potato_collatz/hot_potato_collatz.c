#include <stdbool.h>
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
	int proceso_inicial = 0;
	
	if(argc >= 3)
	{
		papa = strtoll(argv[1], NULL, 10);
		proceso_inicial = strtoll(argv[2], NULL, 10);
	}
	else
	{
		return (void)fprintf(stderr, "error: invalid input\n"), EXIT_SUCCESS;
	}
	
	int next_process = (my_rank + 1) % process_count;
	int prev_process = (my_rank - 1) % process_count;	

	int papa_count = papa;

	if (my_rank == proceso_inicial)
		MPI_Send(&papa, 1, MPI_INT, my_rank, prev_process, MPI_COMM_WORLD);
		
	bool im_alive = true;
	int cont_muertos = 0;
	
	while(cont_muertos != process_count - 1)
	{
		MPI_Recv(&papa, 1, MPI_INT, prev_process, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		if(im_alive)
		{
			if(papa % 2 == 0)
				papa = papa / 2;
			else
				papa = papa * 3 + 1;
		}
		
		if(papa <= 1 && im_alive)
		{
			im_alive = false;
			cont_muertos++;
			papa = papa_count;
		}
		MPI_Allreduce(&cont_muertos, &cont_muertos, 1,MPI_INT, MPI_MAX, MPI_COMM_WORLD);
		
		if(cont_muertos == process_count - 1)
			printf("The winner is %d\n", next_process);
			
		MPI_Send(&papa, 1, MPI_INT, next_process, my_rank, MPI_COMM_WORLD);
	}
	
	MPI_Finalize();
	return 0;
}
