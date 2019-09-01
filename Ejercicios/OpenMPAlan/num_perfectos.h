#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <omp.h> 
 
using namespace std;

bool validaCntHilos(int ch);
bool validaCntTerminos(long long ct);
 
int main(int argc, char* argv[]) {
	int n,i,j,s;
	int thread_count = 0;
 
	while (!validaCntHilos(thread_count) || !validaCntTerminos(n)) {
		cout << "Digite la cantidad de hilos ( >=1 ): ";
		cin >> thread_count;
		cout << "Digite la cantidad de términos ( >=1 ): ";
		cin >> n; //number_of_tosses
	}
 
	for(i=2; i < n; i++)
	{
		s = 0;
		#pragma omp parallel for num_threads(thread_count) default(none) shared(i) reduction(+: s) 
		for (j=1; j < i-1; j++) 
		{
			if (i % j == 0)
				s += j;
		}
		if (i == s)
			cout << n << ", ";
	}
	cout << endl
	cin >> i;
  return 0;
}

bool validaCntHilos(int ch) {
	return ch >= 1;
}

// ct es la cantidad de términos de la sumatoria
bool validaCntTerminos(long long ct) {
	return ct >= 1;
}