#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char* argv[])
{	
	int thread_count = omp_get_num_procs();
	if(argc >= 2)
		thread_count = atoi(argv[1]);
		
	int iterations = thread_count;
	if(argc >= 3)
		iterations = atoi(argv[2]);
	
	#pragma omp parallel num_threads(thread_count) default(none) shared(iterations)
	{
		#pragma omp for
		for ( int iteration = 0; iteration < iterations; ++iteration )
			printf("Stage 1: %d/%d iteration %d/%d\n", omp_get_thread_num(), omp_get_num_threads(), iteration, iterations);
		
		#pragma omp for
		for ( int iteration = 0; iteration < iterations; ++iteration )
			printf("Stage 2: %d/%d iteration %d/%d\n", omp_get_thread_num(), omp_get_num_threads(), iteration, iterations);
		
		#pragma omp for
		for ( int iteration = 0; iteration < iterations; ++iteration )
			printf("Stage 3: %d/%d iteration %d/%d\n", omp_get_thread_num(), omp_get_num_threads(), iteration, iterations);
	}
	return 0;
}
