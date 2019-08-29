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
	
	MPI_Allreduce(&all_sum, &all_sum, 1,MPI_LONG, MPI_SUM, MPI_COMM_WORLD);
	MPI_Allreduce(&all_min, &all_min, 1,MPI_LONG, MPI_SUM, MPI_COMM_WORLD);
	MPI_Allreduce(&all_max, &all_max, 1,MPI_LONG, MPI_SUM, MPI_COMM_WORLD);
		
	if ( random_num == all_min )
		printf("Process %d, min #%ld is the minimum\n", my_rank, all_min);
		
	if ( random_num > all_sum / process_count )
		printf("Process %d, my #%ld is greater than the average %ld\n", my_rank, all_min, all_sum/process_count);
	
		
	MPI_Finalize();
	return 0;
}
