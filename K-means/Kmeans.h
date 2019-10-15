#include <omp.h>
#include <vector>
#include <queue>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <time.h>
#include <cmath>
#include <set>

using namespace std;

#include "Aleatorizador.h"

#define MAXI 2147483647 

class Kmeans
{
    public:
        Kmeans(int cantClusters);

        // Destructor
        ~Kmeans();
		
        vector<int>& getVecAsignaciones();
        vector< vector<double> >& getVecClusters();

        void initCentroides(vector< vector<double> >& vecDatos, double* fi, int* contAsig);
        void kmedias(vector< vector<double> >& vecDatos, int* contAsig);
        void establecerTam(vector< vector<double> >& vecDatos);
        void calcDistance(vector< vector<double> >& vecDatos);
        void calcAsigment(int* contAsig);
        void calcMedia(vector< vector<double> >& vecDatos);
	
    private:
        int cantClusters; // k (cantidad de centroides a seleccionar)
        bool huboAsignaciones;

        vector< vector<double> > clusters; //Centroides k , Nombre tomado por video de DataMining
        vector< vector<double> > clustersCandidatos;
		vector< vector<double> > clustersCandidatosCopy;
        vector<int> clusterAsigment;
		vector< queue<int> > vecAsig;
        vector< vector<double> > distancias;
        vector<int> pesos;		

        void costX(vector< vector<double> >& vecDatos, vector< vector<double> >& subVector, double* sum); //Modifica el valor de sum
        double distanciaEuclideana(vector<double>& valoresVec1, vector<double>& valoresVec2);
        void kmeansPlusplus(int* pos, double* promedioMaximo);
        void calcPesos(vector<vector<double>>& vecDatos);
        int getPesoMayor(int numClsuter); // Retorna la posicion del punto con mayor peso del grupo # numCluster;
};

Kmeans::Kmeans(int cantClusters): cantClusters(cantClusters)
{
	huboAsignaciones = true;
	vecAsig.resize(cantClusters);
}

Kmeans::~Kmeans()
{}

void Kmeans::initCentroides(vector< vector<double> >& vecDatos, double* fi, int* contAsig)
{
	srand(time(NULL));
	Aleatorizador::inicializar_generador_random();

	int ind = rand() % vecDatos.size();
	clustersCandidatos.push_back(vecDatos[ind]);
	clustersCandidatosCopy.push_back(vecDatos[ind]);

	int iter = 0, pos;
	double sum = 0.0;

	#pragma omp parallel num_threads( omp_get_num_procs() * 4 ) shared(vecDatos, iter, fi, sum, pos)
	{
		costX(vecDatos, clustersCandidatos, fi);
		#pragma omp barrier

		#pragma omp single
		*fi = log(*fi);

		while (iter < (int)(*fi) ) //for que se har� logl veces
		{
			costX(vecDatos, clustersCandidatos, &sum);
			#pragma omp barrier

			#pragma omp for
			for (int j = 0; j < vecDatos.size(); j++)
			{
				int tam_clusters;
				double min = MAXI;

				#pragma omp critical
				tam_clusters = (int)clustersCandidatos.size();

				for (int k = 0; k < tam_clusters; k++)
				{
					double dist = distanciaEuclideana(clustersCandidatos[k], vecDatos[j]);
					if (min > dist)
						min = dist;
				}

				#pragma omp critical
				if (Aleatorizador::random_uniform_real(Aleatorizador::generador) < (0.7 * min) / sum)
				{
					clustersCandidatos.push_back(vecDatos[j]);
					clustersCandidatosCopy.push_back(vecDatos[j]);
				}
			}

			#pragma omp single
			iter++;
		}
     
		kmeansPlusplus(&pos, &sum);
	
		kmedias(clustersCandidatos, contAsig);

		calcPesos(clustersCandidatos);

		#pragma omp for
		for (int i = 0; i < cantClusters; i++)
			clusters[i] = clustersCandidatos[getPesoMayor(i)];
	}

}

void Kmeans::kmedias(vector< vector<double> >& vecDatos, int* contAsig)
{
    establecerTam(vecDatos);

    while (huboAsignaciones)
    {
        calcDistance(vecDatos);

        calcAsigment(contAsig);

        if (huboAsignaciones)
            calcMedia(vecDatos);
    }
}

void Kmeans::establecerTam(vector< vector<double> >& vecDatos)
{
    #pragma omp single
    {
		huboAsignaciones = true; //reseteo del boolean
        //Definimos los tamanhos de los vectores que posee la clase
        distancias.resize(vecDatos.size());
        clusterAsigment.resize(vecDatos.size());
    }

    #pragma omp for
    for (int i = 0; i < distancias.size(); i++)
        distancias[i].resize(cantClusters);
}

void Kmeans::calcDistance(vector< vector<double> >& vecDatos)
{
    #pragma omp for
    for(int i = 0; i < cantClusters; i++)
		//#pragma omp for
        for (int j = 0; j < (int)vecDatos.size(); j++)
            distancias[j][i] = distanciaEuclideana(clusters[i] /*centroide*/, vecDatos[j]);
}

void Kmeans::calcAsigment(int* contAsig)
{	
    #pragma omp single
    *contAsig = 0; //Reseteo del contador

    #pragma omp for
    for(int i = 0; i < distancias.size(); i++)
    {
        int ident = 0; //Guardar� el valor del centroide (indice) al que su distancia es la menor
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
            (*contAsig)++;
		}
		else
			clusterAsigment[i] = ident;

		#pragma omp critical
		vecAsig[ident].push(i);
    }

    #pragma omp single
    if (*contAsig == distancias.size())
        huboAsignaciones = false;
}

void Kmeans::calcMedia(vector< vector<double> >& vecDatos)
{
    #pragma omp for
    for(int i = 0; i < cantClusters; i++)
        for (int j = 0; j < clusters[0].size(); j++)
            clusters[i][j] = 0;

	#pragma omp for
	for (int i = 0; i < cantClusters; i++) //i = identificador del cluster
	{
		int cont = vecAsig[i].size();
		while (!vecAsig[i].empty())
		{
			for (int k = 0; k < vecDatos[0].size(); k++)
				clusters[i][k] += vecDatos[vecAsig[i].front()][k];
			vecAsig[i].pop();
		}

		for (int l = 0; l < clusters[0].size(); l++)
			clusters[i][l] /= (double)cont;
	}
}

double Kmeans::distanciaEuclideana(vector<double>& valoresVec1, vector<double>& valoresVec2)
{
    double d_e = 0.0; //guardar� la diferencia al cuadrado de las dimensiones del centroide y un vector
    
    for (int i = 0; i < valoresVec1.size() /*dimensionClusters*/; i++)
        d_e += pow(valoresVec1[i] - valoresVec2[i], 2.0);

    return pow(d_e, 0.5);
}

void Kmeans::costX(vector< vector<double> >& vecDatos, vector< vector<double> >& subVector, double* sum)
{
	#pragma omp single
	*sum = 0.0;

	#pragma omp for
	for (int i = 0; i < vecDatos.size(); i++)
	{
		double min = MAXI;
		for (int j = 0; j < subVector.size(); j++)
		{
			double dist = distanciaEuclideana(subVector[j] /*centroide*/, vecDatos[i]);
			if (min > dist)
				min = dist;
		}
		#pragma omp atomic
		*sum += min;
	}
}

void Kmeans::kmeansPlusplus(int* pos, double* promedioMaximo)
{
    srand(time(NULL));
	
	#pragma omp single
	{
		*pos = rand() % clustersCandidatos.size();
		clusters.push_back(clustersCandidatosCopy[*pos]);
		clustersCandidatosCopy.erase(clustersCandidatosCopy.begin() + *pos);
	}

	while (clusters.size() < cantClusters)
	{
		#pragma omp single
		*promedioMaximo = 0.0;

		#pragma omp for
		for (int i = 0; i < clustersCandidatosCopy.size(); i++)
		{
			double suma = 0;
			for (int j = 0; j < clusters.size(); j++)
			{
				suma += distanciaEuclideana(clustersCandidatosCopy[i], clusters[j]);
			}

			#pragma omp critical
			if (suma > *promedioMaximo)
			{
				*promedioMaximo = suma;
				*pos = i;
			}
		}

		#pragma omp single
		{
			clusters.push_back(clustersCandidatosCopy[*pos]);
			clustersCandidatosCopy.erase(clustersCandidatosCopy.begin() + *pos);
		}
    }
}

void Kmeans::calcPesos(vector< vector<double> >& vecDatos)
{
    #pragma omp single
    pesos.resize(vecDatos.size());

    #pragma omp for
    for (int i = 0; i < vecDatos.size(); i++)
    {
        int pos = (i + 1) % vecDatos.size();
        double menor = distanciaEuclideana(vecDatos[i], vecDatos[pos]);

        for (int j = 0; j < vecDatos.size(); j++)
            if (i != j)
                if (distanciaEuclideana(vecDatos[i], vecDatos[j]) <= menor)
                    pos = j;
        #pragma omp atomic
        pesos[pos]++;
    }
}

int Kmeans::getPesoMayor(int numCluster)
{
	int pesoMayor = -5;
	int pos = 0; //posicion

	for (int i = 0; i < clusterAsigment.size(); i++)
	{
		if (clusterAsigment[i] == numCluster && pesos[i] > pesoMayor)
		{
			pesoMayor = pesos[i];
			pos = i;
		}
	}
	return pos;
}

vector<int>& Kmeans::getVecAsignaciones()
{
    return clusterAsigment;
}

vector< vector<double> >& Kmeans::getVecClusters()
{
    return clusters;
}
