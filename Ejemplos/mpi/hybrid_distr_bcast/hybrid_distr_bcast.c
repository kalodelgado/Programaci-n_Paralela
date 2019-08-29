#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>

#define MIN(a,b) ( (a) < (b) ? (a) : (b))

size_t calc_start(size_t data_count, size_t worker_count, size_t id)
{
	return id * ( (data_count / worker_count) + MIN(id, data_count % worker_count) );
}

size_t calc_finish(size_t data_count, size_t worker_count, size_t id){
	return calc_start(size_t data_count, size_t worker_count, size_t id + 1);
}

int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);
	
	//MPI_Comm_rank(MPI_Comm comunicador, int *rank(id));
	int my_rank = -1;
	int process_count = -1; //Cant de procesos que se van a distribuir
	
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); //rank = process id
	MPI_Comm_size(MPI_COMM_WORLD, &process_count);
	
	char hostname[MPI_MAX_PROCESSOR_NAME];
	int hostname_length = -1;
	MPI_Get_processor_name(hostname, &hostname_length);
	
	size_t start = 0;
	size_t end = 0;
	
	if(argc >= 3)
	{
		start = strtoll(argv[1], NULL, 10);
		end = strtoll(argv[2], NULL, 10);
	}
	else
	{
		if(my_rank == 0)
			if ( scanf("%zu %zu", &start, &end) != 2 )
				return (void)fprintf(stderr, "error: invalid start and finish\n"), EXIT_SUCCESS;
		//Se hace fuera de los if, es decir con cada proceso
		MPI_Bcast(&start, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);
		MPI_Bcast(&end, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);
	}
	
	size_t my_start = start + calc_start(end - start, (size_t)process_count, (size_t)my_rank);
	size_t my_finish = start + calc_finish(end - start, (size_t)process_count, (size_t)my_rank);
	
	size_t size = my_finish - my_start;
	
	printf("%s:%d: range [%zu, %zu[ size %zu\n"
		, hostname, my_rank, start, finish, size);
	
	#pragma omp parallel default(none) shared(my_rank, process_count, hostname, my_start, my_finish)
	{
		size_t thread_start = 0;
		size_t thread_finish = 0;
		
		#pragma omp for
		for (size_t current = my_start; current < my_finish; ++current)
		{
			if(thread_start == 0)
				thread_start = current;
			thread_finish = current;
		}
		++thread_finish;
		
		#pragma omp critical (stdout)
		printf("\t%s:%d, %d: range [%zu, %zu[ size: %zu\n"
			, hostname, my_rank, omp_get_thread_num(), thread_start, thread_finish, thread_finish - thread_start );
	}
	
	MPI_Finalize();
	return 0;
}
