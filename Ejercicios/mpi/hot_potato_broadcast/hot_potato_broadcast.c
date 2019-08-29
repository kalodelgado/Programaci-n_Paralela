#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include <omp.h>

int next_process(int my_rank, int process_count)
{
	return (my_rank + 1) % process_count;
}

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
	int proceso_inicial;
	
	if(argc >= 3)
	{
		papa = strtoll(argv[1], NULL, 10);
		proceso_inicial = strtoll(argv[2], NULL, 10);
	}
	else
	{
		return (void)fprintf(stderr, "error: invalid input\n"), EXIT_SUCCESS;
	}

	int datos[5];
	datos[0] = 1; //next_process
	datos[1] = papa; 
	datos[2] = papa; //initial value of the papa
	datos[3] = 0; //conteo de muertos
	datos[4] = 0; //current player

	if (my_rank == proceso_inicial)
	{
		datos[0] = next_process(my_rank, process_count);
		datos[4] = my_rank;
		datos[5] = (my_rank - 1) % process_count;
	}
		
	MPI_Bcast(&datos, 5, MPI_INT, datos[4], MPI_COMM_WORLD);
		
	bool im_alive = true;
	
	do
	{
		int wait_for = datos[4];
		if(my_rank == datos[4])
		{
			if(im_alive)
			{
				if(datos[1] % 2 == 0)
					datos[1] /= 2;
				else
					datos[1] = datos[1] * 3 + 1;
					
				if(datos[1] == 1)
				{
					im_alive = false;
					datos[3]++;//contador de muertos
					datos[1] = datos[2];
				}
			}
			datos[4] = datos[0];			
			datos[0] = next_process(datos[0], process_count);			 
		}		
		
		if(datos[3] == process_count - 1)
			printf("The winner is %d\n", datos[4]);
			
		MPI_Bcast(&datos, 5, MPI_INT, wait_for, MPI_COMM_WORLD);
	}
	while(datos[3] != process_count - 1);
	
	MPI_Finalize();
	return 0;
}
