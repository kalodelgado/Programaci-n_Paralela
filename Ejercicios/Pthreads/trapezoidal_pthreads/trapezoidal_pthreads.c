#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>

//Thread shared data
typedef struct{
		size_t a;
		size_t n;
		double total_trap;
		double area_total;
		size_t thread_count;
		double (*ptr_func)(double);
	}shared_data_t;

//Thread private data
typedef struct{
		size_t thread_id;
		double area;
		shared_data_t* shared_data;
	}thread_data_t;

double calcular_n_start(size_t n, size_t thread_id, size_t thread_count)
{
	return 1 + (n / thread_count * thread_id);
}

double calcular_n_end(size_t n, size_t thread_id, size_t thread_count)
{
	if (thread_id == thread_count -1)
		return n-1;
	return 1 + (n / thread_count * (thread_id + 1) );
}

void* calculo_trapezoidal(void* data)
{
	thread_data_t* info = (thread_data_t*)data;
	shared_data_t* shared_data = info->shared_data;
	
	double start = calcular_n_start(shared_data->n, info->thread_id, shared_data->thread_count);
	double finish = calcular_n_end(shared_data->n, info->thread_id, shared_data->thread_count);
		
	for(double index = start; index < finish; index++)
	{
		info->area += shared_data->ptr_func( shared_data->a + ( index * shared_data->total_trap ) );
	}
	
	return NULL;
}

double calcular_funcion(double x)
{
	// Witch of Agnesi
	//y(x^2+a^2)=a^3, a = 7
	return pow(7,3) / ( pow(x,2) + pow(7,2) );
}

int main(int argc, char* argv[])
{
	shared_data_t shared_data;
	size_t b;
	if(argc == 4){
		if(sscanf(argv[1],"%zu", &shared_data.a) != 1)
			return (void)fprintf(stderr, "error a %s\n",argv[1]), 1;
		if(sscanf(argv[2],"%zu", &b) != 1)
			return (void)fprintf(stderr, "error b %s\n",argv[2]), 1;
		if ( sscanf(argv[3], "%zu", &shared_data.n) != 1)
			return (void)fprintf(stderr, "error n %s\n", argv[3]), 1;
	}
	
	size_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
	if(shared_data.n < thread_count)
		thread_count = shared_data.n;
	pthread_t* threads = (pthread_t*)malloc( thread_count * sizeof(pthread_t) );
	
	shared_data.total_trap = fabs( b - shared_data.a ) / shared_data.n;	
	shared_data.area_total = ( calcular_funcion(b) + calcular_funcion(shared_data.a) ) / 2;
	shared_data.ptr_func = &calcular_funcion;
	shared_data.thread_count = thread_count;
	
	struct timespec start_time;//creacion de registro
	clock_gettime(CLOCK_MONOTONIC, &start_time);
    
    thread_data_t* thread_data = (thread_data_t*)calloc(thread_count, sizeof(thread_data_t));
    for(size_t index = 0; index < thread_count; ++index){
		thread_data[index].thread_id = index;
		thread_data[index].shared_data = &shared_data;
		
		pthread_create(&threads[index], NULL, calculo_trapezoidal, &thread_data[index]);
	}
	
	for(size_t index = 0; index < thread_count; ++index)
		pthread_join(threads[index], NULL);
		
	double area_threads = 0;
	for(size_t index = 0; index < thread_count; ++index)
		area_threads += thread_data[index].area;
	if(area_threads != 0)
		shared_data.area_total = (area_threads * shared_data.total_trap);
	else
		shared_data.area_total *= shared_data.total_trap;
	
	struct timespec finish_time;
	clock_gettime(CLOCK_MONOTONIC, &finish_time);
	
	printf("\nThe integral area is: %lf\n",shared_data.area_total);
	
	double seconds = finish_time.tv_sec - start_time.tv_sec				//tv.sec; tiempo en segundos; .nsec nanosegundos
				   + (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
	printf("Time: %.8lfs\n",seconds);
	
	free(thread_data); 
	free(threads);
}
