#include <omp.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
using namespace std;

#include "LectorArchivo.h"
#include "Kmeans.h"

int main()
{
    int k = 0; //cantidad de agrupamientos requerida
    int	n = 2; //dimensionalidad de los datos (cantidad de indices en cada vector de datos)
    int	m = 4; //cantidad de vectores de datos
	double start, finish; //Almacenaran el tiempo pared

    do{
        cout << "Digite la cantidad de agrupamientos requerida ( >=1 ): ";
        cin >> k;
    }while(k <= 0);

    do{
        cout << "Digite la dimensionalidad de los datos ( >=1 ): ";
        cin >> n;
    }while(n <= 0);

    do{
        cout << "Digite la cantidad de vectores de datos ( >=1 ): ";
        cin >> m;
    }while(m <= 0);
    system("cls");

    vector< vector<double> > vecDatos;
    vecDatos.resize(m);

    #pragma omp parallel for shared(vecDatos)
    for(int i = 0; i < vecDatos.size(); i++)
		vecDatos[i].reserve(n);

    string nombreArchivo;
    bool noesValido = false;

    do {
        cout << "Digite el nombre del archivo seguido de .csv" << endl;
        cin >> nombreArchivo;
        ifstream archivo(nombreArchivo, ios::in);
        if (!archivo) { // operador ! sobrecargado
            noesValido = true;
            cerr << "Has digitado mal el nombre del archivo o este no es valido \n" << endl;
        } else {
            noesValido = false;
            LectorArchivo lector(archivo, vecDatos);
        }
    } while (noesValido);
	
	start = omp_get_wtime();
	
	Kmeans kmeans(k);
	
	kmeans.initCentroides(vecDatos);
	
	#pragma omp parallel num_threads( omp_get_num_procs() ) shared(vecDatos)
	while( kmeans.getHuboAsignaciones() )
	{
		kmeans.calcDistance(vecDatos);
		kmeans.calcAsigment();
		
		if( kmeans.getHuboAsignaciones() )
			kmeans.calcMedia(vecDatos);
		
		#pragma omp barrier
	}
	
	finish = omp_get_wtime();
	std::cout << endl << "Duracion " << finish - start << " segundos.";
	
	cin >> k;
	return 0;
}
