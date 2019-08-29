#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>

void* run(void* data)
{
	(void)data;
	return NULL;
}

int main(int argc, char* argv[])
{	
	size_t thread_count = 1;
	if(argc > 1){
		if(sscanf(argv[1],"%zu", &thread_count) != 1 || thread_count == 0)
			return (void)fprintf(stderr, "hello_w: error: invalid thread count: %s\n",argv[1]), 1;
	}
	
	pthread_t* threads = (pthread_t*)malloc( thread_count * sizeof(pthread_t) );
	if( threads == NULL)
		return (void)fprintf(stderr, "hellow: error: could not allocate memory for: %zu threads\n", thread_count), 2;
	
	//creacion registros de tiempo
	struct timespec start_time;
	struct timespec finish_time;
	
	double min_time = LONG_MAX;
	for(size_t index = 0; index < thread_count; ++index)
	{		
		clock_gettime(CLOCK_MONOTONIC, &start_time);//para medir tiempo, instruct y registro dond se almacenará
	
		pthread_create(&threads[index], NULL, run, (void*)index);
		pthread_join(threads[index], NULL);
		
		clock_gettime(CLOCK_MONOTONIC, &finish_time);
		
		double nseconds = (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
		if ( nseconds < min_time)
			min_time = nseconds;
	}
	
	printf("Minimum thread creation and destruction time was %.11fs among %zu trials\n",min_time,thread_count);
	
	free(threads);
	
	return 0;
}
