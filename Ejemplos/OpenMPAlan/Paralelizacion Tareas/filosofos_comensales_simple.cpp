#include <omp.h>
#include <vector>
#include <fstream>
#include <chrono>
#include <utility> // para usar pair<>
#include <random>
#include <iostream>
#include <time.h>
#include <stdlib.h>
using namespace std;

enum Estado { hambriento, comiendo, pensando };

void cambiarEstado(vector< Estado >& estados, vector< int >& tenedores, vector< int >& cmc, int id, int* tenMarcado);
bool verDeadlock(vector< Estado >& estados, vector< int >& tenedores);

// para generar numeros aleatorios
static std::default_random_engine generador;
static std::uniform_real_distribution<double> distribution;

int main()
{
	// para acceder al reloj del sistema al generar numeros aleatorios
	typedef std::chrono::high_resolution_clock myclock;
	myclock::time_point beginning = myclock::now();
	
	// para imprimir los resultados
	fstream resultados("resultados.txt", ios::out);

	int cfth = 0; // cantidad de filósofos, tenedores o hilos
	int ci = 0; // cantidad de iteraciones
	int cex = 0; // cantidad de experimentos
	int c_deadlocks = 0; // cantidad de deadlocks hasta t en un experimento
	bool deadlock = false;

	cout << "Digite la cantidad de filosofos/tenedores/hilos: "; cin >> cfth;
	cout << "Digite la cantidad de iteraciones: "; cin >> ci;
	cout << "Digite la cantidad de experimentos: "; cin >> cex;

	// obtain a seed from the timer
	myclock::duration d = myclock::now() - beginning;
	unsigned seed = d.count();
	generador.seed(seed);
	
	srand(time(NULL) * seed);
	int tenMarcado = rand()%cfth;

	// define e inicializa el vector de estados de filosofos
	vector< Estado > estados(cfth, pensando);
	
	// define e inicializa el vector de tenedores todos libres == -1
	vector< int > tenedores(cfth,-1);
	
	// define e inicializa el vector con las cantidades de minutos en que los filosofos lograron comer
	vector< int > cmc(cfth, 0);
	
	for (int ex = 0; ex < cex; ex++)
	{
		# pragma omp parallel num_threads(cfth) shared(estados, tenedores, cmc, distribution, generador, ci, deadlock, tenMarcado)
		{
			int id = omp_get_thread_num();
			deadlock = false;
			int i = 0;
			while (( i < ci ) && ( !deadlock )) // para ejecutar TODAS las iteraciones --> for (int i = 0; i < ci; i++)
			{
				// Si se usa critical en lugar de single, la simulacion se serializa y por tanto NUNCA se presenta un deadlock.
				// Con single, se logra SIMULAR la DESINCRONIZACION natural de los hilos en un sistema operativo y por tanto se 
				// dan los deadlocks.
				# pragma omp single
				cambiarEstado(estados, tenedores, cmc, id, &tenMarcado);
				# pragma omp barrier
				if (id == 0) {
					deadlock = verDeadlock(estados, tenedores);
					if (deadlock) {
						resultados << "ITR-deadlock: " << i << endl;
						for (int i = 0; i < cfth; i++)
							resultados << tenedores[i] << ',';
						resultados << endl;
						c_deadlocks++;
					}
				}
				# pragma omp barrier
				i++;
			}
		}
		
		for (int i = 0; i < cfth; i++)
			resultados << "El filosofo " << i << " comio " << cmc[i] << endl;
		resultados << "**** Cantidad de deadlocks: " << c_deadlocks << endl;
		resultados << "*****************************************************************" << endl;

		// preparacion para el siguiente experimento;
		// obtain a seed from the timer
		myclock::duration d = myclock::now() - beginning;
		seed = d.count() * ex;
		generador.seed(seed);
		distribution.reset();
		for(int i = 0; i < cfth; i++){
			estados[i] = pensando;
			tenedores[i] = -1;
			cmc[i] = 0;
		}
		ca_fh = 0; c_deadlocks = 0;
	}
	resultados.close();
}

// Permite determinar si hay o un deadlock.
// El que todos los filosofos esten hambrientos no es condicion suficiente para deadlock porque es
// posible que alguno de los hambrientos haya recien entrado a ese estado y no ha tenido tiempo de
// ver si sus tenedores estan disponibles o no.
bool verDeadlock(vector< Estado >& estados, vector< int >& tenedores)
{
	int fh = 0;
	for (int i = 0; i < estados.size(); i++)
		estados[i] == hambriento ? fh++ : fh += 0;
	int to = 0;
	for (int i = 0; i < estados.size(); i++)
		tenedores[i] >= 0 ? to++ : to += 0;
	return (fh == estados.size()) && (to == tenedores.size());
}

void cambiarEstado(vector< Estado >& estados, vector< int >& tenedores, vector< int >& cmc, int id, int* tenMarcado)
{
	int cfth = estados.size();
	if (estados[id] == hambriento) {
		if (tenedores[id] == -1) { // tenedor izquierdo está desocupado
			tenedores[id] = id; // agarra el tenedor izquierdo
		}
		if (tenedores[id] == id && tenedores[(id+1)%cfth] == -1 && *tenMarcado == (id+1)%cfth) { // tenedor der está marcado
			tenedores[(id+1)%cfth] = id; // agarra el tenedor marcado
		}
		if (tenedores[(id+1)%cfth] == -1) { // tenedor derecho está desocupado
			tenedores[(id+1)%cfth] = id; // agarra el tenedor derecho
		}
		if (tenedores[(id+1)%cfth] == id && tenedores[id] == -1 && *tenMarcado == id) { // tenedor izq está marcado
			tenedores[id] = id; // agarra el tenedor marcado
		}
		if ((tenedores[id] == id) && (tenedores[(id+1)%cfth] == id)){
			estados[id] = comiendo;
			cmc[id]++;
		}
	}
	else if (estados[id] == pensando) {
		if (distribution(generador) < 0.75)
			estados[id] = hambriento; // OJO: se cambia a hambriento pero no se verfican los tenedores todavia
	}
	else {
		cmc[id]++;
		if (distribution(generador) < 0.5) { // termina de comer
			if(tenedores[id] = id && *tenMarcado == id){
				*tenMarcado = (id+1)%cfth;
			}			
			if(tenedores[(id+1)%cfth] == id && *tenMarcado == (id+1)%cfth){
				*tenMarcado = id;
			}
			
			tenedores[id] = -1; // libera el tenedor izquierdo
			tenedores[(id+1) % cfth] = -1; // libera el tenedor derecho	
			estados[id] = pensando;
		}
	}
}
