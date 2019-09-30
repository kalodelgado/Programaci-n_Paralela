#include <omp.h>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <time.h>
#include <cmath>
using namespace std;

class Kmeans
{
    public:
        Kmeans(int cantClusters);

        // Destructor
        ~Kmeans();
		
		bool getHuboAsignaciones();
		
		void initCentroides(vector< vector<double> >& vecDatos);
		void calcDistance(vector< vector<double> >& vecDatos);
		void calcAsigment();
		void calcMedia(vector< vector<double> >& vecDatos);
	
	private:
		int cantClusters; // k (cantidad de centroides a seleccionar)
		int contAsig; //Contador de asignaciones
		bool huboAsignaciones;
		
		vector< vector<double> > clusters; //Centroides k
		vector<int> clusterAsigment;
		vector< vector<double> > distancias;
		
		int calcular_n_start(int n, int id, int process_count);
		int calcular_n_end(int n, int id, int process_count);
};

Kmeans::Kmeans(int cantClusters): cantClusters(cantClusters)
{
	contAsig = 0;
	huboAsignaciones = true;
    clusters.reserve(cantClusters);
}

Kmeans::~Kmeans()
{}

void Kmeans::initCentroides(vector< vector<double> >& vecDatos)
{
	
}

void Kmeans::calcDistance(vector< vector<double> >& vecDatos)
{
	#pragma omp single
	{
		//Definimos los tamaños de los vectores que posee la clase
		distancias.reserve( vecDatos.size() );
		clusterAsigment.reserve( vecDatos.size() );
	}

    #pragma omp for
    for(int i = 0; i < distancias.size(); i++)
		distancias[i].reserve(cantClusters);
	
	#pragma omp for
	for(int i = 0; i < cantClusters; i++)
		for(int j = 0; j < vecDatos.size(); j++)
		{
			double d_e = 0.0; //guardará la diferencia al cuadrado de las dimensiones del centroide y un vector
			
			for(int k = 0; k < vecDatos[0].size()/*dimensionClusters*/; k++)
				d_e += pow(clusters[i][k] - vecDatos[j][k], 2.0);
	
			distancias[j][i] = pow(d_e, 0.5);
		}
}

void Kmeans::calcAsigment()
{	
	#pragma omp single
	contAsig = 0; //Reseteo del contador
	
	#pragma omp for
	for(int i = 0; i < distancias.size(); i++)
	{
		int ident = 0; //Guardará el valor del centroide (indice) al que su distancia es la menor
		double min = distancias[i][0];		
		for(int j = 1; j < cantClusters; j++)
			if(min > distancias[i][j])
			{
				ident = j;
				min = distancias[i][j];	
			}
			
		if(clusterAsigment[i] == ident)
		{
			#pragma omp atomic
			contAsig++;
		}
		clusterAsigment[i] = ident;
	}
	
	#pragma omp single
	if( contAsig == distancias.size() )
		huboAsignaciones == false;
}

void Kmeans::calcMedia(vector< vector<double> >& vecDatos)
{
	#pragma omp single
	clusters.clear();
	
	for(int i = 0; i < cantClusters; i++)
	{
		int cont = 0;
		#pragma omp for
		for(int j = 0; j < clusterAsigment.size(); j++)
			if(clusterAsigment[j] == i)
			{
				for(int k = 0; k < vecDatos[0].size(); k++)
					clusters[i][k] += vecDatos[j][k];
					
				cont++;
			}
		
		#pragma omp for
		for(int l = 0; l < clusters[0].size(); l++)
			clusters[i][l] /=(double)cont;
	}
}

bool Kmeans::getHuboAsignaciones()
{
	return huboAsignaciones;
}

int Kmeans::calcular_n_start(int n, int id, int process_count)
{
	return (n / process_count * id);
}

int Kmeans::calcular_n_end(int n, int id, int process_count)
{
	if (id == process_count - 1)
		return n;
	return (n / process_count * (id + 1) );
}
