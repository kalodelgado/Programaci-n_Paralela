#include <omp.h>
#include <memory>
#include <vector>
#include <queue>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
using namespace std;

#include "Barbero.h"
#include "ClienteBarbero.h"
#include "Aleatorizador.h"

/*--------------------------------------------------------------------
 * Function:  uso
 * Purpose:   Print a message indicating how program should be started
 *            and terminate.
 */
void uso(char* prog_name);

// Simula agentes con actividad concurrente que son "animados" por medio de hilos.
// La cantidad de hilos se determina por medio de un factor dado por el usuario vía línea de comandos.
// El factor dado se multiplica por la cantidad de procesadores para determinar la cantidad total de 
// hilos a usar durante toda la simulación.
// Si la cantidad de hilos es mayor o igual a la cantidad de agentes, se asigna un hilo por agente.
// Si la cantidad de hilos es menor a la cantidad de agentes, se distribuyen los agentes entre los
// de forma equitativa. El resto de agentes es "animado" por alguno de los hilos al azar.
int main(int argc, char* argv[])
{
	int cp = 0;		// cantidad de procesadores disponibles
	int clxp = 0;	// cantidad de hilos por procesador
	int ch = 0;		// cantidad efectiva de hilos
	int cha = 1;	// cantidad de hilos activos
	int ci = 0;		// cantidad de iteraciones
	int i = 0;		// contador de iteraciones
	int cs = 0;		// cantidad de sillas para clientes en espera
	double prbClientes = 0.0;	// probabilidad de llegada de los clientes, 

	vector< shared_ptr< Agente >> agentes;
	Barbero* barbero_ptr = new Barbero(Barbero::esperando);
	Barbero& barbero = *barbero_ptr;
	agentes.push_back(shared_ptr< Barbero >(barbero_ptr));

	if (argc != 5) uso(argv[0]);
	clxp = stoi(argv[1], NULL, 10);
	if ((clxp <= 0) || (clxp > 5)) uso(argv[0]);
	ci = stoi(argv[2], NULL, 10);
	if (ci <= 0) uso(argv[0]);
	cs = stoi(argv[3], NULL, 10);
	if (cs <= 0) uso(argv[0]);
	prbClientes = stod(argv[4]);
	if ((prbClientes <= 0) || (prbClientes >= 1)) uso(argv[0]);

	cp = omp_get_num_procs();						// obtiene la cantidad de procesadores

	/* initialize random seed: */
	srand(time(NULL));
	/* inicializa el generador de números entre 0 y 1 */
	Aleatorizador::inicializar_generador_random();	// inicializa aleatorizador
	# pragma omp parallel shared(cp, clxp, ch, cha, ci, i, prbClientes, agentes, ClienteBarbero::clientesEsperando, Aleatorizador::generador,  Aleatorizador::random_uniform_real) num_threads(cp * clxp)
	{
		while (i < ci)
		{
			int pid = omp_get_thread_num();
			// master se encarga de crear nuevos clientes de acuerdo con la probabilidad de llegada de los mismos prbClientes
			# pragma omp master
			if (Aleatorizador::random_uniform_real(Aleatorizador::generador) < prbClientes) {
				int tiempoCortandole = rand() % 9 + 2;
				shared_ptr< ClienteBarbero > clienteBarbero_ptr(new ClienteBarbero(ClienteBarbero::esperando, tiempoCortandole));
				agentes.push_back(clienteBarbero_ptr);
				if (agentes.size() < ch)
					cha = agentes.size();
				if (ClienteBarbero::clientesEsperando.size() < cs)
					ClienteBarbero::clientesEsperando.push(clienteBarbero_ptr);
				else ClienteBarbero::msjsClientes[clienteBarbero_ptr->obtId()] = -1;
			}

			// todos los hilos se sincronizan antes de "actuar"
			# pragma omp barrier
			// cada hilo hace su trabajo de animar los agentes que le corresponden
			int delta = agentes.size() / cha;
			if (pid < cha) {
				int agente_inf = pid * delta;
				int agente_sup = (pid + 1) * delta;
				int car = agentes.size() % cha;		// determina la cantidad de agentes remanentes
				for (int j = agente_inf; j < agente_sup; j++)
					agentes[j]->actuar();
			}

			// algún hilo se encarga de animar los agentes restantes: agentes.size() % cha
			# pragma omp single
			for (int j = agentes.size() - agentes.size() % cha; j < agentes.size(); j++)
						agentes[j]->actuar();

			// master imprime estados de agentes e incrementa contador de iteraciones
			# pragma omp master
			{
				cout << "ITR# " << i << endl;
				cout << "BARBERO: " << " estado: " << agentes[0]->estadoString() << endl;
				for (int j = 1; j < agentes.size(); j++)
					cout << "CLIENTE: " << j << " estado: " << agentes[j]->estadoString() << endl;
				i++;
			}
			# pragma omp barrier
		}
	}
	cout << "FIN DE LA SIMULACION..." << endl;
	cin.ignore();
}

void uso(char *prog_name) {
	fprintf(stderr, "uso: %s <cantidad de hilos por procesador> <cantidad de iteraciones> <cantidad de sillas> <prob llegada clientes>\n", prog_name);
	fprintf(stderr, "   cantidad de hilos, debe ser entre 2 y 5\n");
	fprintf(stderr, "   cantidad de iteraciones, debe ser mayor que cero\n");
	fprintf(stderr, "   cantidad de sillas, debe ser mayor que cero\n");
	fprintf(stderr, "   probabilidad de llegada de los clientes, debe ser mayor que cero y menor que uno\n");
	cin.ignore();
	exit(0);
}
