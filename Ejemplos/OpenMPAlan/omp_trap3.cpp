/* File:    omp_trap3.cpp
* Purpose: Estimate definite integral (or area under curve) using the
*          trapezoidal rule.  This version uses a parallel for directive
*
* Input:   a, b, n
* Output:  estimate of integral from a to b of f(x)
*          using n trapezoids.
*
* Compile: g++ -g -Wall -fopenmp -o omp_trap3 omp_trap3.c
* Usage:   ./omp_trap3 <number of threads>
*
* Notes:
*   1.  The function f(x) is hardwired.
*   2.  In this version, it's not necessary for n to be
*       evenly divisible by thread_count.
*
* IPP:  Section 5.5 (pp. 224 and ff.)
*/


#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <omp.h>
using namespace std;

#define _CRT_SECURE_NO_WARNINGS 1 // para deshabilitar errores por uso de funciones deprecated sobre CRT o consola
#pragma warning(disable : 4996)

bool validaCntHilos(int ch);
bool validaCntTrapezoides(int ct, int ch);
double f(double x);    /* Function we're integrating */
double trap(double a, double b, int n, int thread_count);

int main(int argc, char* argv[]) {
	double  global_result = 0.0;  /* Store result in global_result */
	double  a, b;                 /* Left and right endpoints      */
	int     n;                    /* Total number of trapezoids    */
	int     thread_count = 0;

	while (!validaCntHilos(thread_count) || !validaCntTrapezoides(n, thread_count)) {
		cout << "Digite la cantidad de hilos ( >= 1 ): ";
		cin >> thread_count;
		cout << endl;
		cout << "Digite la cantidad de trapezoides que debe ser divisible por la cantidad de hilos: ";
		cin >> n;
		cout << endl;
	}

	cout << "Digite el valor inferior del intervalo de calculo: ";
	cin >> a;
	cout << "Digite el valor superior del intervalo de calculo: ";
	cin >> b;

	global_result = trap(a, b, n, thread_count);

	cout << "Con n = " << n << " trapezoides, el estimado de la integral de a == " << a << " hasta b == " << b << " es: " << global_result << endl;
	cin >> n; // para que la pantalla de consola no cierre inmediatamente
	return 0;
}

bool validaCntHilos(int ch) {
	return ch >= 1;
}

bool validaCntTrapezoides(int ct, int ch) {
	return (ct > 0) && (ch > 0) && (ct % ch == 0);
}

/*------------------------------------------------------------------
* Function:    f
* Purpose:     Compute value of function to be integrated
* Input arg:   x
* Return val:  f(x)
*/
double f(double x) {
	double return_val;

	return_val = x * x;
	return return_val;
}  /* f */

   /*------------------------------------------------------------------
   * Function:    Trap
   * Purpose:     Use trapezoidal rule to estimate definite integral
   * Input args:
   *    a: left endpoint
   *    b: right endpoint
   *    n: number of trapezoids
   * Return val:
   *    approx:  estimate of integral from a to b of f(x)
   */
double trap(double a, double b, int n, int thread_count) {
	double  h, approx;

	h = (b - a) / n;
	approx = (f(a) + f(b)) / 2.0;
#  pragma omp parallel for num_threads(thread_count) reduction(+: approx)
	for (int i = 1; i <= n - 1; i++)
		approx += f(a + i * h);
	approx = h * approx;

	return approx;
}  /* Trap */