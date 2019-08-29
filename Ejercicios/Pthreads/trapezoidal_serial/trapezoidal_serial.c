#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>


void* calculo_trapezoidal(double (*ptr_func)(double), size_t a, size_t b, size_t n)
{
	double area = 0;
	double total_trap = fabs(b-a) / n;
	
	area = (ptr_func(b) + ptr_func(a))/2;
	for(double index = 1; index < n - 1; index++)
	{
		area += ptr_func( a + ( index * total_trap ) );
	}
	area = area * total_trap;
    printf("\nThe integral is: %lf\n",area);
	
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
	size_t a,b,n;
	if(argc == 4){
		if(sscanf(argv[1],"%zu", &a) != 1)
			return (void)fprintf(stderr, "error a %s\n",argv[1]), 1;
		if(sscanf(argv[2],"%zu", &b) != 1)
			return (void)fprintf(stderr, "error b %s\n",argv[2]), 1;
		if ( sscanf(argv[3], "%zu", &n) != 1)
			return (void)fprintf(stderr, "error n %s\n", argv[3]), 1;
	}
	
	struct timespec start_time;//creacion de registro
	clock_gettime(CLOCK_MONOTONIC, &start_time);
	
	//Declarar puntero a funcion
	double (*ptr_func)(double); 
    ptr_func = &calcular_funcion;
    calculo_trapezoidal(ptr_func, a, b, n);	
	
	struct timespec finish_time;
	clock_gettime(CLOCK_MONOTONIC, &finish_time);
	
	double seconds = finish_time.tv_sec - start_time.tv_sec				//tv.sec; tiempo en segundos; .nsec nanosegundos
				   + (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
	printf("Time: %.8lfs\n",seconds);
}
