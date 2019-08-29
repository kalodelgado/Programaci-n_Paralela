#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

//Thread shared data
typedef struct{
		size_t thread_count;
		size_t data_size;
		double* data;
		size_t rounds;
		unsigned producer_delay;
		unsigned consumer_delay;
		sem_t sem_prod;
		sem_t sem_consumer;
	}shared_data_t;

//thread private data
typedef struct{
		shared_data_t* shared_data;
	}thread_data_t;

void* produce(void* data)
{
	shared_data_t* shared_data = (shared_data_t*)data;
	
	for( size_t round = 1; round <= shared_data->rounds; ++round)
	{
		for (size_t index = 0; index < shared_data->data_size; ++index)
		{
			sem_wait(&shared_data->sem_prod);
			
			usleep( rand() % (shared_data->producer_delay + 1) * 1000);
		
			double product = round + (index +1) / 100.0;
			
			shared_data->data[index] = product;
		
			printf("Produced %.2f\n", product);
			
			sem_post(&shared_data->sem_consumer);
		}		
	}
	return NULL;
}

void* consume(void* data)
{
	shared_data_t* shared_data = (shared_data_t*)data;
	
	for( size_t round = 1; round <= shared_data->rounds; ++round)
	{
		for (size_t index = 0; index < shared_data->data_size; ++index)
		{
			sem_wait(&shared_data->sem_consumer);
			
			usleep( rand() % (shared_data->consumer_delay + 1) * 1000 );
		
			printf("Consumed %.2f\n", shared_data->data[index]);
			
			sem_post(&shared_data->sem_prod);
		}		
	}
	return NULL;
}

int main(int argc, char* argv[])
{	
	shared_data_t shared_data;
	shared_data.thread_count = 2;
	
	
	if(argc == 5){
		if(sscanf(argv[1],"%zu", &shared_data.data_size) != 1 || shared_data.data_size == 0)
			return (void)fprintf(stderr, "error size %s\n",argv[1]), 1;
		if(sscanf(argv[2],"%zu", &shared_data.rounds) != 1 || shared_data.rounds == 0)
			return (void)fprintf(stderr, "error rounds %s\n",argv[2]), 1;
		if ( sscanf(argv[3], "%u", &shared_data.producer_delay) != 1)
			return (void)fprintf(stderr, "producer_consumer: error: invalid producer max delay: %s\n", argv[3]), 1;
		if ( sscanf(argv[4], "%u", &shared_data.consumer_delay) != 1)
			return (void)fprintf(stderr, "producer_consumer: error: invalid consumer max delay: %s\n", argv[4]), 1;
	}
	
	shared_data.data = (double*)malloc( shared_data.data_size * sizeof(double) );
	if( shared_data.data == NULL )
		return 6; //Error
	
	pthread_t* threads = (pthread_t*)malloc( shared_data.data_size * sizeof(pthread_t) );
	if( threads == NULL)
		return (void)fprintf(stderr, "hellow: error: could not allocate memory for: %zu threads\n", shared_data.data_size), 2;
	
	sem_init(&shared_data.sem_prod, 0, shared_data.data_size);//semaforo, es compartido, val inicial
	sem_init(&shared_data.sem_consumer, 0, 0);
	
	pthread_create(&threads[0], NULL, produce, &shared_data);
	pthread_create(&threads[1], NULL, consume, &shared_data);
	
	for(size_t index = 0; index < shared_data.thread_count; ++index)
		pthread_join(threads[index], NULL); //Espera a que el thread termine, duerme el main
	
	sem_destroy(&shared_data.sem_consumer);
	sem_destroy(&shared_data.sem_prod);
	free(shared_data.data);
	free(threads); //liberacion de memoria;
	
	return 0;
}
