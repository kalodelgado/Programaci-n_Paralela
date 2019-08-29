#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

//Thread shared data
typedef struct{
		size_t team_count;
		unsigned time1;
		unsigned time2;
		size_t position;
		sem_t* sem_threads;
		pthread_mutex_t mutex;
		pthread_barrier_t barrera;
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
	
	if(info->thread_id > 5){
		pthread_barrier_wait(&shared_data->barrera);
		usleep( rand() % (shared_data->time1 + 1) * 1000);
		sem_post(&shared_data->sem_threads[info->thread_id - shared_data->team_count]);
	}		
	else{
		sem_wait(&shared_data->sem_threads[info->thread_id]);
		usleep( rand() % (shared_data->time2 + 1) * 1000);
		
		pthread_mutex_lock(&shared_data->mutex);
		++shared_data->position;
		if(shared_data->position <= 3)
			printf("Team %zu position %zu\n", info->thread_id, shared_data->position);	
		pthread_mutex_unlock(&shared_data->mutex);
	}	
	
	return NULL;
}

int main(int argc, char* argv[])
{	
	shared_data_t shared_data;
	shared_data.team_count = 6;
	shared_data.position = 0;
	
	if(argc == 2){
		if(sscanf(argv[1],"%zu", &shared_data.team_count) != 1 || shared_data.team_count == 0)
			return (void)fprintf(stderr, "error team size %s\n",argv[1]), 1;
		if(sscanf(argv[2],"%u", &shared_data.time1) != 1 || shared_data.time1 == 0)
			return (void)fprintf(stderr, "error time one %s\n",argv[2]), 1;
		if(sscanf(argv[3],"%u", &shared_data.time2) != 1 || shared_data.time2 == 0)
			return (void)fprintf(stderr, "error time 2 %s\n",argv[3]), 1;
	}
	
	pthread_mutex_init( &shared_data.mutex, NULL );
	pthread_barrier_init(&shared_data.barrera,NULL,shared_data.team_count);
		
	pthread_t* threads = (pthread_t*)malloc( (shared_data.team_count * 2) * sizeof(pthread_t) );
	if( threads == NULL)
		return (void)fprintf(stderr, "error: could not allocate memory for: %zu threads\n", (shared_data.team_count * 2)), 2;
	
	
	shared_data.sem_threads = (sem_t*)malloc( shared_data.team_count * sizeof(sem_t) ); //semaforos
	thread_data_t* thread_data = (thread_data_t*)calloc((shared_data.team_count * 2), sizeof(thread_data_t));
	
	for(size_t index = 0; index < shared_data.team_count; ++index)
		sem_init(&shared_data.sem_threads[index], 0, 0);
	
	for(size_t index = 0; index < (shared_data.team_count * 2); ++index){
		thread_data[index].thread_id = index;
		thread_data[index].shared_data = &shared_data;
		
		pthread_create(&threads[index], NULL, run, &thread_data[index]);
	}
	
	for(size_t index = 0; index < (shared_data.team_count * 2); ++index)
		pthread_join(threads[index], NULL); //Espera a que el thread termine, duerme el main
		
	free(shared_data.sem_threads);
	free(thread_data);
	free(threads); //liberacion de memoria;
	
	pthread_mutex_destroy(&shared_data.mutex);
	pthread_barrier_destroy(&shared_data.barrera);
	
	return 0;
}
