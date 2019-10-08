#include <omp.h>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <time.h>
#include <cmath>

using namespace std;

#include "Aleatorizador.h"

#define MAXI 2147483647 

class Kmeans
{
    public:
        Kmeans(int cantClusters);

        // Destructor
        ~Kmeans();
		
		/*
		 * @brief Verificar si hubo asignaciones
		 * @return Returns true or false
		 * */
        bool getHuboAsignaciones();
        vector<int>& getVecAsignaciones();
        vector< vector<double> >& getVecClusters();

		/*
		 * @brief Initializa los centroides
		 * @param Vector con los X leidos, 
		 * @param Puntero double que almacena el costo de x 
		 * @param Puntero int para verificar asignaciones
		 * */
        void initCentroides(vector< vector<double> >& vecDatos, double* fi, int* contAsig);
        void kmedias(vector< vector<double> >& vecDatos, int* contAsig);
        void establecerTam(vector< vector<double> >& vecDatos);
        void calcDistance(vector< vector<double> >& vecDatos);
        void calcAsigment(int* contAsig);
        void calcMedia(vector< vector<double> >& vecDatos);
	
    private:
		/*
		* @brief Int que almacena el valor k inicial dado por el usuario
		* */
        int cantClusters; // k (cantidad de centroides a seleccionar)
        
        /*
		* @brief Bool que indica si hubo asignaciones nuevas, para verificar si se detiene o no el programa
		* */
        bool huboAsignaciones;

        vector< vector<double> > clusters; //Centroides k , Nombre tomado por video de DataMining
        vector< vector<double> > clustersCandidatos;
        vector<int> clusterAsigment;
        vector< vector<double> > distancias;
        vector<int> pesos;		

        double costX(vector< vector<double> >& vecDatos);
        double distanciaEuclideana(vector<double>& valoresVec1, vector<double>& valoresVec2);
        void kmeansPlusplus();
        void calcPesos(vector<vector<double>>& vecDatos);
        int getPesoMayor(int numClsuter); // Retorna la posicion del punto con mayor peso del grupo # numCluster;
};

Kmeans::Kmeans(int cantClusters): cantClusters(cantClusters)
{
    huboAsignaciones = true;
    clusters.resize(cantClusters);
}

Kmeans::~Kmeans()
{}

void Kmeans::initCentroides(vector< vector<double> >& vecDatos, double* fi, int* contAsig)
{   
	for (int i = 0; i < clusters.size(); i++)
        clusters[i].resize(vecDatos[0].size());

    clusters[0] = vecDatos[3];
    clusters[1] = vecDatos[10];
	clusters[2] = vecDatos[12];
	
	/*
    srand(time(NULL));
    Aleatorizador::inicializar_generador_random();

    #pragma omp single
    {
        clustersCandidatos.push_back( vecDatos[ rand()%vecDatos.size() ] );
        *fi = costX(vecDatos);
    }
    
    for(int i = 0; i < log( int(*fi) ); i++) //for que se har� logl veces
    {
        #pragma omp for
        for(int j = 0; j < vecDatos.size(); j++)
        {
            double min = MAXI;
            
            for (int k = 0; k < clustersCandidatos.size(); k++)
            {
                double dist = distanciaEuclideana(clustersCandidatos[k], vecDatos[j]);
                if(min > dist)
                    min = dist;
            }
    
            if (Aleatorizador::random_uniform_real(Aleatorizador::generador) < (4 * min) / costX(vecDatos) )
            {
                #pragma omp critical
                clustersCandidatos.push_back( vecDatos[j] );
            }
        }
    }
    #pragma omp barrier
     
    #pragma omp single
    kmeansPlusplus();
    
    kmedias(clustersCandidatos, contAsig);
	
	#pragma omp barrier
  
    calcPesos(clustersCandidatos);
    
    #pragma omp for
    for(int i = 0; i < cantClusters; i++)
        clusters[i] = clustersCandidatos[ getPesoMayor(i) ];
    */
}

void Kmeans::kmedias(vector< vector<double> >& vecDatos, int* contAsig)
{
    establecerTam(vecDatos);

    while (getHuboAsignaciones())
    {
        calcDistance(vecDatos);

        calcAsigment(contAsig);
        #pragma omp barrier

        if (getHuboAsignaciones())
            calcMedia(vecDatos);

        #pragma omp barrier
    }
}

void Kmeans::establecerTam(vector< vector<double> >& vecDatos)
{
    #pragma omp single
    {
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
        for (int j = 0; j < vecDatos.size(); j++)
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
        }else
            clusterAsigment[i] = ident;
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
    for(int i = 0; i < cantClusters; i++) //i = identificador del cluster
    {
        int cont = 0;        
        for(int j = 0; j < clusterAsigment.size(); j++)
            if(clusterAsigment[j] == i)
            {
                for(int k = 0; k < vecDatos[0].size(); k++)
                    clusters[i][k] += vecDatos[j][k];
                cont++;
            }
        
        for(int l = 0; l < clusters[0].size(); l++)
            clusters[i][l] /=(double)cont;
    }
}

double Kmeans::costX(vector< vector<double> >& vecDatos)
{
    double sum = 0; // fi
    double min, dist;

    for(int i = 0; i < vecDatos.size(); i++)
    {
        min = MAXI;
        for (int j = 0; j < clusters.size(); j++)
        {
            dist = distanciaEuclideana(clusters[j] /*centroide*/, vecDatos[i]);
            if(min > dist)
                min = dist;
        }
        sum += min;
    }

    return sum;
}

double Kmeans::distanciaEuclideana(vector<double>& valoresVec1, vector<double>& valoresVec2)
{
    double d_e = 0.0; //guardar� la diferencia al cuadrado de las dimensiones del centroide y un vector
    
    for (int i = 0; i < valoresVec1.size() /*dimensionClusters*/; i++)
        d_e += pow(valoresVec1[i] - valoresVec2[i], 2.0);

    return pow(d_e, 0.5);
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

void Kmeans::kmeansPlusplus()
{
    srand(time(NULL));
    Aleatorizador::inicializar_generador_random();

    clusters.push_back(clustersCandidatos[rand() % clustersCandidatos.size()]);

    while (clusters.size() < cantClusters)
    {
        int ind = rand() % clustersCandidatos.size(); //Obtencion del identificador

        double dmin = MAXI;

        for (int i = 0; i < clusters.size(); i++)
        {
            double dist = distanciaEuclideana(clusters[i], clustersCandidatos[ind]);
            if (dmin > dist)
                dmin = dist;
        }

        if (Aleatorizador::random_uniform_real(Aleatorizador::generador) < dmin / costX(clustersCandidatos))
            clusters.push_back(clustersCandidatos[ind]);
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

bool Kmeans::getHuboAsignaciones()
{
    return huboAsignaciones;
}

vector<int>& Kmeans::getVecAsignaciones()
{
    return clusterAsigment;
}

vector< vector<double> >& Kmeans::getVecClusters()
{
    return clusters;
}
