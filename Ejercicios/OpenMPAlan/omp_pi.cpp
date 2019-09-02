/* File:     omp_pi.cpp
* Purpose:  Estimate pi using OpenMP and the formula
*
*              pi = 4*[1 - 1/3 + 1/5 - 1/7 + 1/9 - . . . ]
*
* Compile:  g++ -g -Wall -fopenmp -o omp_pi omp_pi.c -lm
* Run:      omp_pi <thread_count> <n>
*           thread_count is the number of threads
*           n is the number of terms of the series to use
*
* Input:    none
* Output:   The estimate of pi and the value of pi computed by the
*           arctan function in the math library
*
* Notes:
*    1.  The radius of convergence is only 1.  So the series converges
*        *very* slowly.
*
* IPP:   Section 5.5.4 (pp. 229 and ff.)
*/

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <omp.h> 
using namespace std;

#define _CRT_SECURE_NO_WARNINGS 1 // para deshabilitar errores por uso de funciones deprecated sobre CRT o consola
#pragma warning(disable : 4996)

bool validaCntHilos(int ch);
bool validaCntTerminos(long long ct);

int main(int argc, char* argv[]) {
	long long n, i = 0;
	int thread_count = 0;
	double number_in_circle = 0;
	double x, y;
	double pi_estimate = 0.0;
	srand(time(NULL));

	// captura y valida la cantidad de hilos y términos por pantalla:
	while (!validaCntHilos(thread_count) || !validaCntTerminos(n)) {
		cout << "Digite la cantidad de hilos ( >=1 ): ";
		cin >> thread_count;
		cout << "Digite la cantidad de términos ( >=1 ): ";
		cin >> n; //number_of_tosses
	}

	// realiza el cálculo de pi:
	#  pragma omp parallel for num_threads(thread_count) reduction(+: number_in_circle) private(x, y)
	for (i = 0; i < n; i++) {
		x = -1 + rand() % 2;
		y = -1 + rand() % 2;
		if (x*x + y*y <= 1)
			number_in_circle++;
	}
	pi_estimate = 4.0*number_in_circle/(double)n;

	cout << "Con " << n << " términos y " << thread_count << " hilos" << endl;
	cout << "el estimado de pi es " << pi_estimate << endl;
	cout << "el valor de pi es " << 4.0*atan(1.0) << endl;
	cin >> i; // necesario para que la pantalla de la consola no cierre al final inmediatamente.
	return 0;
}

bool validaCntHilos(int ch) {
	return ch >= 1;
}

// ct es la cantidad de términos de la sumatoria
bool validaCntTerminos(long long ct) {
	return ct >= 1;
}