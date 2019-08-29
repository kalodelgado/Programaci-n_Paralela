#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

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
	
	srand (my_rank * time(NULL));
	long random_num = (rand() % 90) + 10;
	long all_sum = -1;
	long all_min = -1;
	long all_max = -1;
	
	MPI_Reduce(&random_num, &all_min, /*count (cant datos que enviará)*/ 1 /*long*/, MPI_LONG, MPI_MIN,/*root*/ 0, MPI_COMM_WORLD);
	MPI_Reduce(&random_num, &all_max, 1 /*long*/, MPI_LONG, MPI_MAX,/*root*/ 0, MPI_COMM_WORLD);
	MPI_Reduce(&random_num, &all_sum, 1 /*long*/, MPI_LONG, MPI_SUM,/*root*/ 0, MPI_COMM_WORLD);
	
	if(my_rank == 0)
	{
		printf("Min: %ld\n", all_min);
		printf("Max: %ld\n", all_max);
		printf("Prom: %ld\n", all_sum / process_count);
	}
		
	MPI_Finalize();
	return 0;
}
