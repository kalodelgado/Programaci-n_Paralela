#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono> // para medir el tiempo de ejecuci�n
#include <omp.h> 
using namespace std;

void leeAdyacencias(ifstream& ae, vector< vector< int > >& ma, int& cntVertices);
void algoritmoFloydWarshall(const vector< vector< int > >& ma, vector< vector< int > >& mc);

int main() {
	string nombreArchivoEntrada = "redPeq.txt"; // formato *.txt, por ejemplo "grafo.txt
	string nombreArchivoSalida;  // formato *.txt, por ejemplo matriz_costos.txt 
	ifstream archivoEntrada(nombreArchivoEntrada, ios::in);
	ofstream archivoSalida(nombreArchivoSalida, ios::out);
	vector< vector< int > > matrizAdyacencias;
	vector< vector< int > > matrizCostos;
	int cntVertices;
	double start, finish;

	if (!archivoEntrada) { // operador ! sobrecargado
		cout << "No se pudo abrir el archivo de entrada" << endl;
		cin.ignore();
		return 1;
	}

	// se cargan y despliegan las adyacencias del archivo
	leeAdyacencias(archivoEntrada, matrizAdyacencias, cntVertices);
	cout << "MATRIZ DE ADYACENCIAS" << endl;
	for (int i = 0; i < cntVertices; i++) {
		for (int j = 0; j < cntVertices; j++)
			cout << matrizAdyacencias[i][j] << ',';
		cout << endl;
	}
	
	// se toma una marca de tiempo:
	start = omp_get_wtime();
	// se genera la matriz de costos
	algoritmoFloydWarshall(matrizAdyacencias, matrizCostos);

	// se toma otra marca de tiempo
	finish = omp_get_wtime();

	// se despliega la duraci�n:
	std::cout << endl << "Duracion " << finish - start << " segundos.";
	std::cout << std::endl;

	// se despliega la matriz de costos:
	cout << endl << "MATRIZ DE COSTOS" << endl;
	for (int i = 0; i < cntVertices; i++) {
		for (int j = 0; j < cntVertices; j++)
			cout << matrizCostos[i][j] << ',';
		cout << endl;
	}
	cin.ignore(); // para que la consola no se cierre sin ver los resultados al ejecutar desde Visual Studio
}

void leeAdyacencias(ifstream& ae, vector< vector< int > >& ma, int& cntVertices) {
	int pe;
	char finLinea = ' ';

	ae >> cntVertices; // el primer n�mero del archivo es la cantidad de v�rtices
	vector< int > v;
	v.resize(cntVertices, INT_MAX);
	ma.resize(cntVertices, v);

	ae.get(); // consume un blanco
	finLinea = ae.peek(); // intenta leer fin de l�nea

	// salta a la siguiente para leer el primer n�mero de la l�nea #2
	ae >> pe;
	for (int i = 0; i < cntVertices; i++) {
		while (!ae.eof() && (finLinea != '\n')) { // 10 ascii de fin de l�nea
			ma[i][pe] = 1;
			ae >> pe;
			ae.get(); // consume un blanco
			finLinea = ae.peek(); // intenta leer fin de l�nea
		}
		if (!ae.eof())
			ma[i][pe] = 1;
		ae >> pe;
		ae.get();
		finLinea = ae.peek();
	}
}

void algoritmoFloydWarshall(const vector< vector< int > >& ma, vector< vector< int > >& mc) {
	mc = ma;

	#pragma omp parallel for num_threads( omp_get_num_procs() ) default(none) shared(ma, mc)
    for(int i = 0; i < int(ma[0].size()); i++)
        mc[i][i] = 0;
		
    #pragma omp parallel for num_threads( 2 ) default(none) shared(ma, mc)
    for(int k = 0; k < int(ma[0].size()); k++)
        for(int i = 0; i < int(ma[0].size()); i++)
            for(int j = 0; j < int(ma[0].size()); j++){
                int dt = mc[i][k] + mc[k][j];
                if(mc[i][j] > dt)
					#pragma omp critical
                    mc[i][j] = dt;
            }
}
