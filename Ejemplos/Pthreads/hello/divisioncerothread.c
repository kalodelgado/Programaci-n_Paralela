#include <stdio.h>
#include <pthread.h>

/* Doc
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
    void *(*start_routine)(void*), void *arg);
*/

void* run(void* data)
{
	int num = 45;
	printf("Hello from thread %d\n", num);
	return NULL; // como return 0
}

int main(void)
{
	pthread_t thread;
	pthread_create(&thread, NULL, run, NULL); //NULL cuando no se van a usar parametros

	printf("Hello from main thread\n");
	pthread_join(thread, NULL); //Espera a que el thread termine, duerme el main
	return 0;
}
