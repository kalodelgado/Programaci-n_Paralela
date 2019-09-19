/* File:
*     omp_mat_vect.cpp
*
*
* Purpose:
*     Computes a parallel matrix-vector product.  Matrix
*     is distributed by block rows.  Vectors are distributed by
*     blocks.  Unless the DEBUG flag is turned on this version
*     uses a random number generator to generate A and x.
*
* Input:
*     None unless compiled with DEBUG flag.
*     With DEBUG flag, A, x
*
* Output:
*     y: the product vector
*     Elapsed time for the computation
*
* Compile:
*    g++ -g -Wall -o omp_mat_vect omp_mat_vect.c -lpthread
* Usage:
*    omp_mat_vect <thread_count> <m> <n>
*
* Notes:
*     1.  Storage for A, x, y is dynamically allocated.
*     2.  Number of threads (thread_count) should evenly divide both
*         m and n.  The program doesn't check for this.
*     3.  We use a 1-dimensional array for A and compute subscripts
*         using the formula A[i][j] = A[i*n + j]
*     4.  Distribution of A, x, and y is logical:  all three are
*         globally shared.
*     5.  DEBUG compile flag will prompt for input of A, x, and
*         print y
*     6.  Uses the OpenMP library function omp_get_wtime() to
*         return the time elapsed since some point in the past
*
* IPP:    Section 5.9 (pp. 253 and ff.)
*/

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <omp.h> 
using namespace std;

#define _CRT_SECURE_NO_WARNINGS 1 // para deshabilitar errores por uso de funciones deprecated sobre CRT o consola
#pragma warning(disable : 4996)

//#define DEBUG 1

bool validaCntHilos(int ch);
bool validaDimensiones(int& m, int& n);
void gen_matrix(vector<double>& A, int m, int n);
void read_matrix(string prompt, vector<double>& A, int m, int n);
void gen_vector(vector<double>& x, int n);
void read_vector(string prompt, vector<double>& x, int n);
void print_matrix(string title, const vector<double>& A, int m, int n);
void print_vector(string title, const vector<double>& y, double m);

/* Parallel function */
void omp_mat_vect(const vector<double>& A, const vector<double>& x, vector<double>& y, int m, int n, int thread_count);

int main(int argc, char* argv[]) {
	int     thread_count = 0;
	int     m, n = 0;
	vector<double> A;
	vector<double> x;
	vector<double> y;

	while (!validaCntHilos(thread_count) || !validaDimensiones(m, n)) {
		cout << "Digite la cantidad de hilos ( >= 1 ): ";
		cin >> thread_count;
		cout << "Digite m( >= 0 ): ";
		cin >> m;
		cout << "Digite n( >= 0 ): ";
		cin >> n;
	}
	A.resize(m*n,0);
	x.resize(n,0);
	y.resize(m,0);

# ifdef DEBUG
	read_matrix("Enter the matrix", A, m, n);
	print_matrix("We read", A, m, n);
	read_vector("Enter the vector", x, n);
	print_vector("We read", x, n);
# else
	gen_matrix(A, m, n);
	/*    Print_matrix("We generated", A, m, n); */
	gen_vector(x, n);
	/*    Print_vector("We generated", x, n); */
# endif

	omp_mat_vect(A, x, y, m, n, thread_count);

#  ifdef DEBUG
	print_vector("El producto es: ", y, m);
#  else
	print_vector("El producto es: ", y, m);
#  endif

	cin >> m;
	return 0;
}  /* main */


bool validaCntHilos(int ch) {
	return ch >= 1;
}

bool validaDimensiones(int& m, int& n) {
	return (m >= 0) || (n >= 0);
}

/*------------------------------------------------------------------
* Function: gen_matrix
* Purpose:  Use the random number generator random to generate
*    the entries in A
* In args:  m, n
* Out arg:  A
*/
void gen_matrix(vector<double>& A, int m, int n) {
	int i, j;
	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++)
			A[i*n + j] = rand() / ((double)RAND_MAX); // random no aparece en stdlib.h
}  /* gen_matrix */

   /*------------------------------------------------------------------
   * Function:    read_matrix
   * Purpose:     Read in the matrix
   * In args:     prompt, m, n
   * Out arg:     A
   */
void read_matrix(string prompt, vector<double>& A, int m, int n) {
	int             i, j;

	cout << prompt << endl;
	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++)
			cin >> A[i*n + j];
}  /* read_matrix */

   /*------------------------------------------------------------------
   * Function: gen_vector
   * Purpose:  Use the random number generator random to generate
   *    the entries in x
   * In arg:   n
   * Out arg:  x
   */
void gen_vector(vector<double>& x, int n) {
	int i;
	for (i = 0; i < n; i++)
		x[i] = rand() / ((double)RAND_MAX); // random no aparece en stdlib.h
}  /* gen_vector */

   /*------------------------------------------------------------------
   * Function:        read_vector
   * Purpose:         Read in the vector x
   * In arg:          prompt, n
   * Out arg:         x
   */
void read_vector(string prompt, vector<double>& x, int n) {
	int   i;

	cout << prompt << endl;
	for (i = 0; i < n; i++)
		cin >> x[i];
}  /* read_vector */

   /*------------------------------------------------------------------
   * Function:    print_matrix
   * Purpose:     Print the matrix
   * In args:     title, A, m, n
   */
void print_matrix(string title, const vector<double>& A, int m, int n) {

	cout << title << endl;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			cout << A[i*n + j] << ',';
		cout << endl;
	}
}  /* print_matrix */

   /*------------------------------------------------------------------
   * Function:    print_vector
   * Purpose:     Print a vector
   * In args:     title, y, m
   */
void print_vector(string title, const vector<double>& y, double m) {

	cout << title << endl;
	for (int i = 0; i < m; i++)
		cout << y[i] << ',';
	cout << endl;
}  /* print_vector */

   /*------------------------------------------------------------------
   * Function:  omp_mat_vect
   * Purpose:   Multiply an mxn matrix by an nx1 column vector
   * In args:   A, x, m, n, thread_count
   * Out arg:   y
   */
void omp_mat_vect(const vector<double>& A, const vector<double>& x, vector<double>& y,
	int m, int n, int thread_count) {
	double start, finish, elapsed;

	start = omp_get_wtime();
	#  pragma omp parallel for num_threads(thread_count)  default(none) shared(A, x, y, m, n)
	for (int i = 0; i < m; i++) 
	{
		y[i] = 0.0;
		for (int j = 0; j < n; j++)
			y[i] += A[i*n + j] * x[j];
	}
	finish = omp_get_wtime();
	elapsed = finish - start;
	cout << "Tiempo transcurrido en segundos: " << elapsed << endl;

}  /* omp_mat_vect */