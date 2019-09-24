#include <omp.h>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <stdlib.h>
#include <time.h>
using namespace std;

#include "Aleatorizador.h"

class Duende
{
	public:
		Duende();
		virtual ~Duende();
};

void atender_duendes_restantes(queue<string>& cola)
{
	int iter = cola.size() / 3;
	for(int i = 0; i < iter; i++)
	{
		cola.pop();
		cola.pop();
		cola.pop();
	}
}

int main()
{
	int thread_count = 0;
	double proba_venados = 0.0;
	double proba_duendes = 0.0;
	int iter = 0;

	do{
		cout << "Digite la cantidad de hilos ( >=1 ): ";
		cin >> thread_count;
	}while(thread_count <= 0);
	
	do{
		cout << "Digite la cantidad de iteraciones ( >=1 ): ";
		cin >> iter;
	}while(iter <= 0);
	
	do{
		cout << "Digite la probabilidad de estancamiento de un duende ( 0 < x < 1 ): ";
		cin >> proba_duendes;
	}while(proba_duendes <= 0 || proba_duendes >= 1);
	
	do{
		cout << "Digite la probabilidad de que llegue un reno ( 0 < x < 1 ): ";
		cin >> proba_venados;
	}while(proba_venados <= 0 || proba_venados >= 1);	
	system("cls");

	int cant_renos = 0;
	bool ya_repartio = false; //variable que nos dice si los renos ya fueron utilizados
	queue< string > duendes;
	string estado_santa = "descansando";	
	
	srand(time(NULL));	
	Aleatorizador::inicializar_generador_random();
	
	int cont = 0; //contador de iteraciones
	#pragma omp parallel num_threads(thread_count * (1 + rand()%3) ) shared(cont, iter, proba_venados, proba_duendes, cant_renos, duendes, estado_santa, ya_repartio)
	{
		int mid = omp_get_thread_num();
		while( cont < iter)
		{		
			#pragma omp single
			cout << "\nITR: " << cont << endl;
							
			if(mid != 0)
			{
				#pragma omp critical
				if(cant_renos < 9)
					if (Aleatorizador::random_uniform_real(Aleatorizador::generador) < proba_venados)
						cant_renos++;
				
				if (Aleatorizador::random_uniform_real(Aleatorizador::generador) < proba_duendes) 
				{
					#pragma omp critical
					duendes.push("duende");
				}				
			}
			//omitimos la barrera, porque deberian seguir creandose duendes aunque santa esté atendiendo
			#pragma omp master
			{
				#pragma omp critical
				if(cant_renos == 9)				
				{
					if(estado_santa == "partio con los renos")
						estado_santa = "descansando";
					if (ya_repartio == false)
					{
						cout << "Cantidad de duendes vistos por santa antes de partir: " << duendes.size() << endl;
						atender_duendes_restantes(duendes);
						estado_santa = "partio con los renos";
						ya_repartio = true;
					}
				}
												
				#pragma omp critical
				if(duendes.size() >= 3)
				{
					cout << "Cantidad de duendes vistos por santa: " << duendes.size() << endl;
					if(estado_santa == "descansando")
					{
						estado_santa = "atendiendo duendes";
						duendes.pop();
						duendes.pop();
						duendes.pop();
					}
				}
				
			}
			#pragma omp barrier
			
			#pragma omp master
			{
				cout << "Estado de Santa: " << estado_santa << endl;
				cout << "Duendes en espera (al final de iteración): " << duendes.size() << endl;
				cout << "Cantidad de renos (al final de iteración): " << cant_renos << endl;
				if(ya_repartio)
					cout << "	Ya utilizó los renos" << endl;
				else
					cout << "	Aun no utiliza los renos" << endl;
					
				if(estado_santa == "atendiendo duendes")
				{
					cout << "	Santa pasa a descansar" << endl;
					estado_santa == "descansando";
				}
					
				cont++;
			}
			
			#pragma omp barrier
		}
	}
	cout << endl;
	cin >> iter;
}
