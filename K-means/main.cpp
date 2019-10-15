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
    int k = 5; //cantidad de agrupamientos requerida
    int	m = 100; //cantidad de vectores de datos
    double tiempo_pared; //Almacenaran el tiempo pared
    int contAsig = 0; //Contador de asignaciones, para metodo calcAsigment
    double fi = 0.0; //Costo de X respecto con C
    /*
    do{
        cout << "Digite la cantidad de agrupamientos requerida ( >=1 ): ";
        cin >> k;
    }while(k <= 0);

    do{
        cout << "Digite la cantidad de vectores de datos ( >=1 ): ";
        cin >> m;
    }while(m <= 0);
    system("cls");
    */
    vector< vector<double> > vecDatos;
    vecDatos.resize(m);

    string nombreArchivo = "5_100_4.csv";
    
    //cout << "Digite el nombre del archivo seguido de .csv" << endl;
    //cin >> nombreArchivo;
    ifstream archivo(nombreArchivo, ios::in);    
    if (!archivo) { // operador ! sobrecargado
        cerr << "Has digitado mal el nombre del archivo o este no es valido \n" << endl;
        exit(1);
    }
      
    LectorArchivo lector(archivo, vecDatos);
    Kmeans kmeans(k);
	
    tiempo_pared = omp_get_wtime();

	kmeans.initCentroides(vecDatos, &fi, &contAsig);

    #pragma omp parallel num_threads( omp_get_num_procs() * 4 ) shared(vecDatos, contAsig, fi)
    kmeans.kmedias(vecDatos, &contAsig);

	tiempo_pared = omp_get_wtime() - tiempo_pared;
    
    lector.escribirSalida(vecDatos, kmeans.getVecClusters(), kmeans.getVecAsignaciones() , tiempo_pared, fi);

    return 0;
}
