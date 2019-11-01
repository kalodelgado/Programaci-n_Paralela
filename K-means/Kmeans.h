#include <iostream>
#include <vector>
#include <time.h>
#include <math.h>
#include <omp.h>

using namespace std;
#define MAXI 2147483647

class Kmeans 
{
	public:
		/*
		 * @param m int Longitud del vector
		 * */
		Kmeans(int m);
		
		// Destructor
        ~Kmeans();
		
		void llenarCandidatos(vector<vector<double>> &datos, double l);
		void llenarIniciales(int k, vector<vector<double>> &datos);
		void calcAsignaciones(vector<vector<double>> &datos, bool *cambio);
		void recalcIniciales(vector<vector<double>> &datos, int epsilon);
		
		vector<int>& getVecAsignaciones();
        vector< vector<double> >& getVecClusters();
		
		double fi;
	
	private:
		vector<int> pesos;
		vector<int> asignaciones; //Vector donde se verificará si hubo o no asignaciones
		vector<vector<double>> cIniciales; //Centroides k
		vector<vector<double>> cCandidatos;
	
		double distancia(vector<double> &vec1, vector<double> &vec2);
		void calcPesos(vector< vector<double> >& cCandidatos, vector < vector<double>> & datos);
		void calcMedia(vector<vector<double>> &dato);
		void costoX(double *suma, vector<vector<double>> &datos); //Modifica el valor de sum con el coste
		double costoInicial(vector<vector<double>> &datos);
		double costoFinal(vector<vector<double>> &datos);
		void costoMnimo(double *min, int pos, vector<vector<double>> &datos);
};

Kmeans::Kmeans(int m)
{
	fi = 0.0;
	asignaciones.resize(m);
}

Kmeans::~Kmeans()
{}

void Kmeans::llenarCandidatos(vector<vector<double>> &datos, double l)
{
	srand(time(NULL));
	cCandidatos.push_back(datos[rand() % datos.size()]);
	fi = pow(costoInicial(datos), 0.5);

	double min = 0.0, suma = 0.0;

	for (int i = 0; i < 6; i++) //For que se aconseja hacer poco mas de hasta 5 veces
	{
		costoX(&suma, datos);
		
		for (int j = 0; j < datos.size(); j++)
		{
			costoMnimo(&min, j, datos);

			if ((rand() / (RAND_MAX + 1.0)) < ( l * min / suma))
			{
				cCandidatos.push_back(datos[j]);
			}
		}
	}
}

double Kmeans::distancia(vector<double> &vec1, vector<double> &vec2)
{
	double dis = 0.0;
 	for (int i = 0; i < vec1.size(); i++)
		dis += pow(vec1[i] - vec2[i], 2.0);

	return dis;
}

void Kmeans::calcPesos(vector< vector<double> >& cCandidatos, vector < vector<double>> & datos)
{
	pesos.resize(cCandidatos.size());

	#pragma omp parallel for num_threads(omp_get_num_procs() * 3) shared(datos)
	for (int i = 0; i < datos.size(); i++)
	{
		int pos = 0;
		double dis = 0.0, menor = MAXI;
		
		for (int j = 1; j < cCandidatos.size(); j++)
		{
			if ((dis = distancia(datos[i], cCandidatos[j])) <= menor)
			{
				if (dis != double(0))
				{
					pos = j;
					menor = dis;
				}
			}
		}
		pesos[pos]++;
	}
}

void Kmeans::costoX(double *suma, vector<vector<double>> &datos)
{

	double minimo = MAXI, dis=0.0;
	*suma = 0.0;

	#pragma omp parallel for num_threads( omp_get_num_procs() * 3) shared(datos, suma, minimo)
	for (int i = 0; i < datos.size(); i++)
	{
		double minimo = MAXI;
		for (int j = 0; j < cCandidatos.size(); j++)
		{
			if ((dis = distancia(datos[i], cCandidatos[j])) < minimo)
			{
				minimo = dis;
			}
		}
		*suma += minimo;
	}

}

void Kmeans::costoMnimo(double *min, int pos, vector<vector<double>> &datos)
{
	*min = MAXI;
	double minimo = MAXI, dis=0.0;
	
	for (int i = 0; i < cCandidatos.size(); i++)
	{
		if ((minimo = distancia(cCandidatos[i], datos[pos])) < *min)
		{
			*min = minimo;
		}
	}
}

void Kmeans::llenarIniciales(int k, vector<vector<double>> &datos)
{
	calcPesos(cCandidatos, datos);
	cIniciales.resize(k);

	int cantidad = 0, mayor = 0, pos = 0;
	for (int j = 0; j < k; j++)
	{
		cantidad = 0;
		mayor = 0;
		pos = 0;
		
		for (int i = 0; i< cCandidatos.size() ; i++)
		{
			if ((cantidad = pesos[i]) >= mayor)
			{
				mayor = cantidad;
				pos = i;
			}
		}
		cIniciales[j]=cCandidatos[pos];
		pesos[pos] = -100;
	}
}

void Kmeans::recalcIniciales(vector<vector<double>> &datos, int epsilon)
{
    bool cambio = true;
	double costo1 = 0.0, costo2=0.0;
	
	do
	{
		costo1 = costoFinal(datos);
		calcMedia(datos);
		calcAsignaciones(datos, &cambio);
		costo2 = costoFinal(datos);
	} while (abs(costo1 - costo2) > epsilon&& cambio);
	
}

 void Kmeans::calcMedia(vector<vector<double>> &datos)
{
	//Operacion muy barata como para paralelizar
	 for (int i = 0; i < cIniciales.size(); i++)
		 for (int j = 0; j < cIniciales[0].size(); j++)
			 cIniciales[i][j] = 0;

	#pragma omp parallel for num_threads(omp_get_num_procs() * 3) shared(datos)
	 for (int i = 0; i < cIniciales.size(); i++)
	 {
		 int cont = 0;
		 for (int j = 0; j < datos.size(); j++)
		 {
			 if (asignaciones[j] == i)
			 {
				 for (int k = 0; k < cIniciales[0].size(); k++)
				 {
					 cIniciales[i][k] += datos[j][k];
					 
				 }
				 cont++;
			 }
		 }

		 for (int l = 0; l < cIniciales[0].size(); l++)
			 cIniciales[i][l] /= (double)cont;
	 }	 
}

void Kmeans::calcAsignaciones(vector<vector<double>> &datos, bool *cambio)
{
	*cambio = false; //Reseteo del booleano

	#pragma omp parallel for num_threads(omp_get_num_procs() * 3) shared (datos, cambio)
	for (int i = 0; i < datos.size(); i++)
	{
		int pos = 0;
		double menor = MAXI, dis = 0.0;
		
		for (int j = 0; j < cIniciales.size(); j++)
		{
			if ( (dis = distancia(datos[i], cIniciales[j])) <= menor )
			{
				pos = j;
				menor = dis;
			}		
		}

		if( asignaciones[i] != pos )
		{
			asignaciones[i] = pos;
			*cambio = true;
		}
	}
}

double Kmeans::costoInicial(vector<vector<double>> &datos)
{
	double suma=0.0;
	for (int i = 0; i < datos.size(); i++)
		suma += distancia(cCandidatos[0], datos[i]);

	return suma;
}

double Kmeans::costoFinal(vector<vector<double>> &datos)
{
	double suma = 0.0, dis=0.0;

	#pragma omp parallel for num_threads(omp_get_num_procs() * 3) shared(dis,datos) reduction(+:suma)
	for (int i = 0; i < datos.size(); i++)
	{
		double minimo = MAXI;
		for (int j = 0; j < cIniciales.size(); j++)
		{
			if ( ( dis = distancia(datos[i], cIniciales[j]) ) < minimo)
				minimo = dis;
		}
		suma += minimo;
	}
	return suma;
}

vector<int>& Kmeans::getVecAsignaciones()
{
    return asignaciones;
}

vector< vector<double> >& Kmeans::getVecClusters()
{
    return cIniciales;
}
