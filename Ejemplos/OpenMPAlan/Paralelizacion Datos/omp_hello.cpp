/* File:     omp_hello.cpp
*
* Purpose:  A parallel hello, world program that uses OpenMP
*
* Compile:  g++ -g -Wall -fopenmp -o omp_hello omp_hello.c
* Run:      ./omp_hello <number of threads>
*
* Input:    none
* Output:   A message from each thread
*
* IPP:      Section 5.1 (pp. 211 and ff.)
*/

#include <iostream>
#include <omp.h> 
using namespace std;

#define _CRT_SECURE_NO_WARNINGS 1 // para deshabilitar errores por uso de funciones deprecated sobre CRT o consola
#pragma warning(disable : 4996)

void hello();  /* Thread function */
bool validaCntHilos(int ch);

int main(int argc, char* argv[]) {
	int thread_count = 0;

	while (!validaCntHilos(thread_count)) {
		cout << "Digite la cantidad de hilos ( >=1 ): ";
		cin >> thread_count;
	}

	# pragma omp parallel num_threads(thread_count)
	hello();

	cin.ignore(); cin.ignore(); // así es: doble ignore()!!
	return 0;
}  /* main */

bool validaCntHilos(int ch) {
	return ch >= 1;
}

/*-------------------------------------------------------------------
 * Function:    hello
 * Purpose:     Thread function that prints message
 */
void hello() {
   int my_rank = omp_get_thread_num();
   int thread_count = omp_get_num_threads();

   //printf("Hello from thread %d of %d\n", omp_get_thread_num(), omp_get_num_threads());
	#pragma omp critical
   cout << "Hello from thread " << my_rank <<  " of " << thread_count << "\n" << endl;

}  /* Hello */