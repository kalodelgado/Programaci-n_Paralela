#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

/* Doc
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
    void *(*start_routine)(void*), void *arg);
*/

typedef struct{
	size_t thread_id;
	size_t total_threads;
	}thread_data_t;

void* run(void* data)
{
	thread_data_t* info = (thread_data_t*)data;
	size_t thread_id = info->thread_id;
	size_t total_threads = info->total_threads;
	printf("Hello from thread %zu of %zu\n", thread_id + 1, total_threads);
	return NULL; // como return 0
}

int main(int argc, char* argv[])
{	
	size_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);//cantidad de threads
	if(argc > 1){
		if(sscanf(argv[1],"%zu", &thread_count) != 1 || thread_count == 0)
			return (void)fprintf(stderr, "hello_w: error: invalid thread count: %s\n",argv[1]), 1;
	}
	
	pthread_t* threads = (pthread_t*)malloc( thread_count * sizeof(pthread_t) );
	if( threads == NULL)
		return (void)fprintf(stderr, "hellow: error: could not allocate memory for: %zu threads\n", thread_count), 2;
	
	thread_data_t* thread_data = (thread_data_t*)calloc(thread_count, sizeof(thread_data_t));
	for(size_t index = 0; index < thread_count; ++index){
		//thread[index] = {index, thread_count};
		thread_data[index].thread_id = index;
		thread_data[index].total_threads = thread_count;
		
		//direccion de memoria de &
		pthread_create(&threads[index], NULL, run, &thread_data[index]);
	}

	printf("Hello from main thread\n");
	
	for(size_t index = 0; index < thread_count; ++index)
		pthread_join(threads[index], NULL); //Espera a que el thread termine, duerme el main
	
	free(thread_data); 
	free(threads); //liberacion de memoria;
	
	return 0;
}
