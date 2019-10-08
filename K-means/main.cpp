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
    int k = 3; //cantidad de agrupamientos requerida
    int	n = 5; //dimensionalidad de los datos (cantidad de indices en cada vector de datos)
    int	m = 14; //cantidad de vectores de datos
    double start, finish; //Almacenaran el tiempo pared
    int contAsig = 0; //Contador de asignaciones, para metodo calcAsigment
    double fi = 0.0; //Costo de X respecto con C
    /*
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
    */
    vector< vector<double> > vecDatos;
    vecDatos.resize(m);

    #pragma omp parallel for shared(vecDatos)
    for(int i = 0; i < (int)vecDatos.size(); i++)
        vecDatos[i].reserve(n);
	
    string nombreArchivo = "prub.txt";
    
    //cout << "Digite el nombre del archivo seguido de .csv" << endl;
    //cin >> nombreArchivo;
    ifstream archivo(nombreArchivo, ios::in);    
    if (!archivo) { // operador ! sobrecargado
        cerr << "Has digitado mal el nombre del archivo o este no es valido \n" << endl;
        exit(1);
    }
      
    LectorArchivo lector(archivo, vecDatos);
    Kmeans kmeans(k);
	
    start = omp_get_wtime();

	#pragma omp parallel num_threads( omp_get_num_procs() ) shared(vecDatos, contAsig, fi)
	{
		kmeans.initCentroides(vecDatos, &fi, &contAsig);

		kmeans.kmedias(vecDatos, &contAsig);
	}

    finish = omp_get_wtime();
    std::cout << "Duracion " << finish - start << " segundos." << endl;
    
    lector.escribirSalida(vecDatos, kmeans.getVecClusters(), kmeans.getVecAsignaciones() );

    return 0;
}
