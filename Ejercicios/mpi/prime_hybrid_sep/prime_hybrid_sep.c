#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>
#include <mpi.h>
#include <omp.h>

bool is_prime(size_t number)
{
	if ( number < 2 ) return false;
	if ( number == 2 ) return true;
	if ( number % 2 == 0 ) return false;

	for ( size_t i = 3, last = (size_t)(double)sqrt(number); i <= last; i += 2 )
		if ( number % i == 0 )
			return false;

	return true;
}

size_t calcular_n_start(size_t n, size_t id, size_t process_count)
{
	return (n / process_count * id);
}

size_t calcular_n_end(size_t n, size_t id, size_t process_count)
{
	if (id == process_count - 1)
		return n;
	return (n / process_count * (id + 1) );
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
	
	size_t my_start = start + calcular_n_start(end - start, (size_t)my_rank, (size_t)process_count);
	size_t my_finish = start + calcular_n_end(end - start, (size_t)my_rank, (size_t)process_count);

	size_t prime_count = 0;

	double start_time = MPI_Wtime();
	#pragma omp parallel default(none) shared(my_rank, process_count, hostname, my_start, my_finish, prime_count)
	{
		#pragma omp for
		for ( size_t current = my_start; current <= my_finish; ++current )
			if ( is_prime(current) )
			{
				#pragma omp atomic
				++prime_count;
			}
	}
	double finish_time = MPI_Wtime();
	
	double seconds = finish_time - start_time;
	
	fprintf(stdout, "Process %d on %s found %zu primes in range [%zu, %zu[ in %.8lfs with %d threads\n"
					, my_rank, hostname, prime_count, my_start, my_finish, seconds, omp_get_thread_num());
	MPI_Finalize();
	return 0;
}
