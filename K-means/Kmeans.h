#include <omp.h>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <time.h>
#include <cmath>

using namespace std;

#define MAXI 2147483647 

class Kmeans
{
    public:
        Kmeans(int cantClusters);

        // Destructor
        ~Kmeans();
		
        bool getHuboAsignaciones();
        vector<int>& getVecAsignaciones();
        vector< vector<double> >& getVecClusters();

        void initCentroides(vector< vector<double> >& vecDatos);
        void calcDistance(vector< vector<double> >& vecDatos);
        void calcAsigment(int* contAsig);
        void calcMedia(vector< vector<double> >& vecDatos);
	
    private:
        int cantClusters; // k (cantidad de centroides a seleccionar)
        bool huboAsignaciones;

        vector< vector<double> > clusters; //Centroides k , Nombre tomado por video de DataMining
        vector<int> clusterAsigment;
        vector< vector<double> > distancias;

        double costX(vector< vector<double> >& vecDatos);
        double distanciaEuclideana(int centroide, vector<double>& valoresVec);
};

Kmeans::Kmeans(int cantClusters): cantClusters(cantClusters)
{
    huboAsignaciones = true;
    clusters.resize(cantClusters);
}

Kmeans::~Kmeans()
{}

void Kmeans::initCentroides(vector< vector<double> >& vecDatos)
{   
    //srand(time(NULL));
    //Aleatorizador::inicializar_generador_random();

    for (int i = 0; i < clusters.size(); i++)
        clusters[i].resize(vecDatos[0].size());

    clusters[0] = vecDatos[3];
    clusters[1] = vecDatos[10];
    clusters[2] = vecDatos[12];

    /*
    clusters.push_back( clusters[ rand()%cantClusters ] );

    double fi = costX(vecDatos);

    #pragma omp parallel num_threads( omp_get_num_procs() ) shared(vecDatos, min)
    for(int i = 0; i < log(fi); i++) //for que se har� logl veces
    {
        #pragma omp for
        for(int j = 0; j < vecDatos.size(); j++)
        {
            min = MAXI;
            for (int k = 0; k < clusters.size(); k++)
            {
                double dist = distanciaEuclideana(k, vecDatos[j]);
                #pragma omp critical
                if(min > dist)
                    min = dist;				
            }
            if (Aleatorizador::random_uniform_real(Aleatorizador::generador) < (fi * min) / costX(vecDatos) )
                clusters.push_back( vecDatos[j] );
        }
    }
    */
}

void Kmeans::calcDistance(vector< vector<double> >& vecDatos)
{
    #pragma omp single
    {
        //Definimos los tama�os de los vectores que posee la clase
        distancias.resize( vecDatos.size() );
        clusterAsigment.resize( vecDatos.size() );
    }

    #pragma omp for
    for(int i = 0; i < distancias.capacity(); i++)
        distancias[i].resize(cantClusters);

    #pragma omp for
    for(int i = 0; i < cantClusters; i++)
        for (int j = 0; j < vecDatos.size(); j++)
            distancias[j][i] = distanciaEuclideana(i /*centroide*/, vecDatos[j]);
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
    for(int i = 0; i < cantClusters; i++)
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
            dist = distanciaEuclideana(j /*centroide*/, vecDatos[i]);
            if(min > dist)
                min = dist;
        }
        sum += min;
    }

    return sum;
}

double Kmeans::distanciaEuclideana(int centroide, vector<double>& valoresVec)
{
    double d_e = 0.0; //guardar� la diferencia al cuadrado de las dimensiones del centroide y un vector
    
    for (int i = 0; i < valoresVec.size() /*dimensionClusters*/; i++)
        d_e += pow(clusters[centroide][i] - valoresVec[i], 2.0);

    return pow(d_e, 0.5);
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
