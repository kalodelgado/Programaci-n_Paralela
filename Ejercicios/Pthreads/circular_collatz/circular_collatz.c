#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct{
		size_t worker_count;
		size_t number_count;
		size_t* numbers;
		size_t current_step;
		size_t max_steps;
		size_t contador;
		bool workers_done;
		pthread_mutex_t mutex;
		pthread_barrier_t barrier;
	}shared_data_t;
	
typedef struct{
		size_t thread_id;
		shared_data_t* shared_data;
	}thread_data_t;
	
size_t calcular_n_start(size_t n, size_t thread_id, size_t thread_count)
{
	return (n / thread_count * thread_id);
}

size_t calcular_n_end(size_t n, size_t thread_id, size_t thread_count)
{
	if (thread_id == thread_count -1)
		return n;
	return (n / thread_count * (thread_id + 1) );
}

void* calculate(void* data) 
{
	thread_data_t* info = (thread_data_t*)data;
	shared_data_t* shared_data = info->shared_data;
	
	size_t start = calcular_n_start(shared_data->number_count, info->thread_id, shared_data->worker_count);
	size_t finish = calcular_n_end(shared_data->number_count, info->thread_id, shared_data->worker_count);
	
	while ( shared_data->current_step < shared_data->max_steps && !shared_data->workers_done)
	{
		for (size_t index = start; index < finish; ++index)
		{
			const size_t next = (index + 1) % shared_data->number_count;
			const size_t prev = (size_t)((long long)index - 1) % shared_data->number_count;
			
			if(shared_data->numbers[index] > 1)
			{
				pthread_mutex_lock(&shared_data->mutex);
				
				if ( shared_data->numbers[index] % 2 == 0 )
					shared_data->numbers[index] /= 2;
				else
					shared_data->numbers[index] = shared_data->numbers[prev] * shared_data->numbers[index] + shared_data->numbers[next];
					
				pthread_mutex_unlock(&shared_data->mutex);
			}
		}
		pthread_barrier_wait(&shared_data->barrier);
		
		shared_data->contador = 0;
		for(size_t index = start; index < finish; index++)
			if(shared_data->numbers[index] == 1)
			{
				pthread_mutex_lock(&shared_data->mutex);
				shared_data->contador++;
				pthread_mutex_unlock(&shared_data->mutex);
			}

		pthread_barrier_wait(&shared_data->barrier);

		if ( info->thread_id == 0 )
		{
			if(shared_data->contador == shared_data->number_count)
				shared_data->workers_done = true;
			else
				++shared_data->current_step;
		}
	}
	return NULL;
}

int main(int argc, char* argv[])
{
	shared_data_t shared_data;
	shared_data.workers_done = false;
	if(argc == 4){
		if(sscanf(argv[1],"%zu", &shared_data.number_count) != 1)
			return (void)fprintf(stderr, "error a %s\n",argv[1]), 1;
		if(sscanf(argv[2],"%zu", &shared_data.max_steps) != 1)
			return (void)fprintf(stderr, "error b %s\n",argv[2]), 1;
		if ( sscanf(argv[3], "%zu", &shared_data.worker_count) != 1)
			return (void)fprintf(stderr, "error n %s\n", argv[3]), 1;
	}else
		return (void)fprintf(stderr, "collatz: error faltan arguments\n"), 1;

	shared_data.numbers = (size_t*)malloc( shared_data.number_count * sizeof(size_t) );
	if( shared_data.numbers == NULL)
		return (void)fprintf(stderr, "collatz: error: could not allocate memory for: %zu numbers\n", shared_data.number_count), 2;
		
	pthread_t* workers = (pthread_t*)malloc(shared_data.worker_count * sizeof(pthread_t));
	if( workers == NULL)
		return (void)fprintf(stderr, "collatz: error: could not allocate memory for: %zu workers\n", shared_data.worker_count), 3;
	
	//Llena el vector de numeros
	for ( size_t index = 0; index < shared_data.number_count; ++index ){
		scanf("%zu", &shared_data.numbers[index]);
    }
    printf("\n");

	pthread_barrier_init(&shared_data.barrier, NULL, (unsigned)shared_data.worker_count);
	pthread_mutex_init(&shared_data.mutex, NULL);
	
	//cambiamos worker_count de ser necesario
	if(shared_data.number_count < shared_data.worker_count)
		shared_data.worker_count = shared_data.number_count;
	
	//Creamos los threads
	thread_data_t* thread_data = (thread_data_t*)calloc(shared_data.worker_count, sizeof(thread_data_t));
	for ( size_t index = 0; index < shared_data.worker_count; ++index )
	{
		thread_data[index].thread_id = index;
		thread_data[index].shared_data = &shared_data;
		pthread_create(&workers[index], NULL, calculate, &thread_data[index]);
	}

	for ( size_t index = 0; index < shared_data.worker_count; ++index )
		pthread_join(workers[index], NULL);

	if ( shared_data.current_step > shared_data.max_steps )
		printf("No converge in %zu steps\n", shared_data.max_steps);
	else
		printf("Converged in %zu steps\n", shared_data.current_step);
		
	pthread_barrier_destroy(&shared_data.barrier);
	pthread_mutex_destroy(&shared_data.mutex);
	free(thread_data); 
	free(shared_data.numbers);
	free(workers);

	return 0;
}
