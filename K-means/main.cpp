#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <time.h>
#include <omp.h>

#include "LectorArchivo.h"
#include "Kmeans.h"

using namespace std;

int main()
{
	int k = 1; //cantidad de agrupamientos requerida
    int	m = 1; //cantidad de vectores de datos
    double tiempo_pared; //Almacenaran el tiempo pared
	double e = 0; //Epsilon
	bool asignacion = false; //Boolean que nos indica si hay asignaciones
	
    do{
        cout << "Digite la cantidad de agrupamientos requerida ( >=1 ): ";
        cin >> k;
    }while(k <= 0);

    do{
        cout << "Digite la cantidad de vectores de datos ( >=1 ): ";
        cin >> m;
    }while(m <= 0);
	
	do{
        cout << "Digite el epsilon ( e > 500 ): ";
        cin >> e;
    }while(e < 500);
    system("cls");

	vector< vector<double> > vecDatos;
    vecDatos.resize(m);

	string nombreArchivo;
    
    cout << "Digite el nombre del archivo seguido de .csv" << endl;
    cin >> nombreArchivo;
    ifstream archivo(nombreArchivo, ios::in);    
    if (!archivo) { // operador ! sobrecargado
        cerr << "Has digitado mal el nombre del archivo o este no es valido \n" << endl;
    }

	LectorArchivo lector(archivo, vecDatos);
	Kmeans kmeans(m); //Se envia m porque es la longitud total de vectores
	e = 750;
	
	tiempo_pared = omp_get_wtime();

	kmeans.llenarCandidatos(vecDatos, 0.42 * k); // datos y l

	kmeans.llenarIniciales(k, vecDatos);
	
	kmeans.calcAsignaciones(vecDatos, &asignacion);

	kmeans.recalcIniciales(vecDatos, e);

	tiempo_pared = omp_get_wtime() - tiempo_pared;

	lector.escribirSalida(vecDatos, kmeans.getVecClusters(), kmeans.getVecAsignaciones(), tiempo_pared, kmeans.fi);

	return 0;
}
