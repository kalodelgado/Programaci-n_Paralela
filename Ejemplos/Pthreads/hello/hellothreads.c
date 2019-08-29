#include <stdio.h>
#include <pthread.h>

/* Doc
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
    void *(*start_routine)(void*), void *arg);
*/

void* run(void* data)
{
	size_t thread_id = (size_t)data;
	printf("Hello from thread %zu\n", thread_id);
	return NULL; // como return 0
}

int main(void)
{
	pthread_t thread;
	pthread_create(&thread, NULL, run, (void*)7); //NULL cuando no se van a usar parametros, 7 funje como id
	
	printf("Hello from main thread\n");
	pthread_join(thread, NULL); //Espera a que el thread termine, duerme el main
	return 0;
}
