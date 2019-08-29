#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

/* Doc
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
    void *(*start_routine)(void*), void *arg);
*/
//Thread shared data
typedef struct{
		size_t thread_count;
		size_t position;
		pthread_mutex_t mutex;
	}shared_data_t;

//thread private data
typedef struct{
		size_t thread_id;
		shared_data_t* shared_data;
	}thread_data_t;

void* run(void* data)
{
	thread_data_t* info = (thread_data_t*)data;
	shared_data_t* shared_data = info->shared_data;
	
	pthread_mutex_lock(&shared_data->mutex);
	++shared_data->position;
	
	printf("Thread %zu/%zu I arrived at position %zu\n"
	, info->thread_id + 1, shared_data->thread_count, shared_data->position);	
	pthread_mutex_unlock(&shared_data->mutex);
	return NULL;
}

int main(int argc, char* argv[])
{	
	size_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);//cantidad de threads
	if(argc > 1)
		if(sscanf(argv[1],"%zu", &thread_count) != 1 || thread_count == 0)
			return (void)fprintf(stderr, "hello_w: error: invalid thread count: %s\n",argv[1]), 1;
	
	pthread_t* threads = (pthread_t*)malloc( thread_count * sizeof(pthread_t) );
	if( threads == NULL)
		return (void)fprintf(stderr, "hellow: error: could not allocate memory for: %zu threads\n", thread_count), 2;
	
	shared_data_t shared_data;
	shared_data.thread_count = thread_count;
	shared_data.position = 0;
	
	pthread_mutex_init(&shared_data.mutex, NULL);
	
	thread_data_t* thread_data = (thread_data_t*)calloc(thread_count, sizeof(thread_data_t));
	for(size_t index = 0; index < thread_count; ++index){
		//thread_data[index] = {index, thread_count};
		thread_data[index].thread_id = index;
		thread_data[index].shared_data = &shared_data;
		
		pthread_create(&threads[index], NULL, run, &thread_data[index]);
	}
	
	pthread_mutex_lock(&shared_data.mutex);
	printf("Hello from main thread\n");	
	pthread_mutex_unlock(&shared_data.mutex);
	
	for(size_t index = 0; index < thread_count; ++index)
		pthread_join(threads[index], NULL); //Espera a que el thread termine, duerme el main
	
	pthread_mutex_destroy(&shared_data.mutex);
	free(thread_data); 
	free(threads); //liberacion de memoria;
	
	return 0;
}
