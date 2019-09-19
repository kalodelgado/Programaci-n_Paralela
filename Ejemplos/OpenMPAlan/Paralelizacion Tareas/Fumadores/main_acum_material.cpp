#include <omp.h>
#include <iostream>
#include <string>
using namespace std;

#include "Aleatorizador.h"

typedef enum { esperando, haciendo_cigarrillo, fumando } T_estado_fumador;

/*--------------------------------------------------------------------
 * Function:  uso
 * Purpose:   Print a message indicating how program should be started
 *            and terminate.
 */
void uso(char* prog_name);

string estadoFumador(T_estado_fumador ef);

// Simula el círculo de fumadores.
// master o h0 es el proveedor de los tres insumos para hacer cigarros.
// h1 que tiene tabaco, h2 que tiene papel y h3 que tiene cerillas;
// son los tres fumadores.
// Parámetros pasados por línea de comandos:
// 1. cantidad de iteraciones
// 2. A = probabilidad de aprovisionar por parte de master o h0,
// 3. TP = probabilidad de que master provea tabaco y papel,
// 4. PC = probabilidad de que master provea papel y cerillas,
// 5. TC = probabilidad de que master provea tabaco y cerillas.
// 6. EF = probabilidad de que un fumador empiece a fumar cuando ha estado preparando un cigarrillo
// 7. VE = probabilidad de que un fumador vuelva a esperar cuando ha estado fumando
int main(int argc, char* argv[])
{
	int it = 0;					// contador de iteraciones
	int ci = 0;					// cantidad de iteraciones provista por línea de comandos.

	// representa la probabilidad de empezar a fumar dado que el fumador ha estado preparando un cigarrillo.
	double prbEmpezarFumar = 0.0; 
	
	// fumador[0] representa el estado del que tiene tabaco --> h1
	// fumador[1] representa el estado del que tiene papel --> h2
	// fumador[2] representa el estado del que tiene cerillas --> h3
	T_estado_fumador stdo_fumadores[3] = {esperando, esperando, esperando};
	
	// probabilidades[0] == A del evento PROVEER del master o agente
	// probabilidades[1] == TP del evento Tabaco y Papel que provee el agente
	// probalilidades[2] == PC del evento Papel y Cerillas que provee el agente
	// probabilidades[3] == TC del evento Tabaco y Cerillas que provee el agente
	// probabilidades[4] == EF del evento "empieza a fumar" cuando un fumador ha estado preparando un cigarrillo
	// probabilidades[5] == VE del evento "vuelve a empezar" cuando un fumador ha estado fumando
	double probabilidades[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

	// materialesProvistos[0] ==> Tabaco ==> T
	// materialesProvistos[1] ==> Papel ==> P
	// materialesProvistos[2] ==> Cerillas ==> C
	int materialesProvistos[3] = { 0, 0, 0 };	// arreglo de materiales provistos por h0 en cada iteración

	if (argc != 8) uso(argv[0]);
	else {
		ci = strtol(argv[1], NULL, 10);
		//probabilidades[0] = strtod(*argv, &pEnd);
		probabilidades[0] = atof(argv[2]);
		if ((probabilidades[0] < 0) || (probabilidades[0] >= 1)) uso(argv[0]);
		//probabilidades[1] = strtod(*argv, &pEnd);
		probabilidades[1] = atof(argv[3]);
		if ((probabilidades[1] < 0) || (probabilidades[1] >= 1)) uso(argv[0]);
		//probabilidades[2] = strtod(*argv, &pEnd);
		probabilidades[2] = atof(argv[4]);
		if ((probabilidades[2] < 0) || (probabilidades[2] >= 1)) uso(argv[0]);
		//probabilidades[3] = strtod(*argv, &pEnd);
		probabilidades[3] = atof(argv[5]);
		if ((probabilidades[3] < 0) || (probabilidades[3] >= 1)) uso(argv[0]);
		//probabilidades[4] = strtod(*argv, &pEnd);
		probabilidades[4] = atof(argv[6]);
		if ((probabilidades[4] < 0) || (probabilidades[4] >= 1)) uso(argv[0]);
		//probabilidades[5] = strtod(*argv, &pEnd);
		probabilidades[5] = atof(argv[7]);
		if ((probabilidades[5] < 0) || (probabilidades[5] >= 1)) uso(argv[0]);

		// se acumulan las probabilidades para efectos de selección posterior del evento por parte de master
		probabilidades[2] += probabilidades[1];
		probabilidades[3] += probabilidades[2];
	}

	Aleatorizador::inicializar_generador_random();	// inicializa aleatorizador

	// se asignan cuatro hilos al equipo: h0, h1, h2 y h3.
	# pragma omp parallel num_threads(4) shared (it, ci, probabilidades, materialesProvistos, stdo_fumadores)
	{
		int mid = omp_get_thread_num();
		while (it < ci)
		{
			# pragma omp master
			{
				double cA = Aleatorizador::random_uniform_real(Aleatorizador::generador);
				if (cA < probabilidades[0]) { // si h0 debe aprovisionar algún material
					double cM = Aleatorizador::random_uniform_real(Aleatorizador::generador);
					if (cM < probabilidades[1]) {  // debe aprovisionar TP
						materialesProvistos[0]++;
						materialesProvistos[1]++;
					}
					else if (cM < probabilidades[2]) { // debe aprovisionar PC
						materialesProvistos[1]++;
						materialesProvistos[2]++;
					}
					else { // debe aprovisionar TC
						materialesProvistos[0]++;
						materialesProvistos[2]++;
					}
				}
			}
			// todos esperan a que master haga su trabajo
			# pragma barrier

			#pragma omp single
			{
				cout << "\nITR# " << it << endl;
				cout << "Provistos: " << endl;
				if (materialesProvistos[0] > 0 || materialesProvistos[1] > 0 || materialesProvistos[2] > 0)
				{
					if (materialesProvistos[0])
						cout << "	Cantidad tabaco: " << materialesProvistos[0] << endl;
					if (materialesProvistos[1])
						cout << "	Cantidad papel: " << materialesProvistos[1] << endl;
					if (materialesProvistos[2])
						cout << "	Cantidad cerillas: " << materialesProvistos[2] << endl;
				}
				else
				{
					cout << "	No hubo" << endl;
				}
			}

			if (mid == 1) {	// comportamiento del h1 que tiene tabaco
				if (stdo_fumadores[0] == esperando)
					#pragma omp critical
					if (materialesProvistos[1] > 0 && materialesProvistos[2] > 0)
					{
						stdo_fumadores[0] = haciendo_cigarrillo;
						materialesProvistos[1]--;
						materialesProvistos[2]--;
					}
					else;
				else
					if (stdo_fumadores[0] == haciendo_cigarrillo) {
						double ef = Aleatorizador::random_uniform_real(Aleatorizador::generador);
						if (ef < probabilidades[4])		// pasa a fumar
							stdo_fumadores[0] = fumando;
					}
					else {
						double ve = Aleatorizador::random_uniform_real(Aleatorizador::generador);
						if (ve < probabilidades[5])	// vuelve a esperar ingredientes para hacer cigarrillo
							stdo_fumadores[0] = esperando;
					}
			}

			if (mid == 2) {	// comportamiento del h2 que tiene papel
				if (stdo_fumadores[1] == esperando)
					#pragma omp critical
					if (materialesProvistos[0] > 0 && materialesProvistos[2] > 0)
					{
						stdo_fumadores[1] = haciendo_cigarrillo;
						materialesProvistos[0]--; 
						materialesProvistos[2]--;
					}
					else;
				else
					if (stdo_fumadores[1] == haciendo_cigarrillo) {
						double ef = Aleatorizador::random_uniform_real(Aleatorizador::generador);
						if (ef < probabilidades[4])		// pasa a fumar
							stdo_fumadores[1] = fumando;
					}
					else {
						double ve = Aleatorizador::random_uniform_real(Aleatorizador::generador);
						if (ve < probabilidades[5])	// vuelve a esperar ingredientes para hacer cigarrillo
							stdo_fumadores[1] = esperando;
					}
			}

			if (mid == 3) {	// comportamiento del h3 que tiene cerillas
				if (stdo_fumadores[2] == esperando)
					#pragma omp critical
					if (materialesProvistos[0] > 0 && materialesProvistos[1] > 0)
					{
						stdo_fumadores[2] = haciendo_cigarrillo;
						materialesProvistos[0]--; 
						materialesProvistos[1]--;
					}
					else;

				else
					if (stdo_fumadores[2] == haciendo_cigarrillo) {
						double ef = Aleatorizador::random_uniform_real(Aleatorizador::generador);
						if (ef < probabilidades[4])		// pasa a fumar
							stdo_fumadores[2] = fumando;
					}
					else {
						double ve = Aleatorizador::random_uniform_real(Aleatorizador::generador);
						if (ve < probabilidades[5])	// vuelve a esperar ingredientes para hacer cigarrillo
							stdo_fumadores[2] = esperando;
					}
			}			
			// Este barrier sólo es necesario para que master imprima el estado de los fumadores sin inconsistencias
			# pragma omp barrier

			# pragma omp master
			{
				cout << "Estado del fumador con tabaco: " << estadoFumador(stdo_fumadores[0]) << endl;
				cout << "Estado del fumador con papel: " << estadoFumador(stdo_fumadores[1]) << endl;
				cout << "Estado del fumador con cerillas: " << estadoFumador(stdo_fumadores[2]) << endl;
			}

			# pragma omp single
			it++;

			# pragma omp barrier
		}
	}
	cin.ignore();
}

void uso(char *prog_name) {
	fprintf(stderr, "uso: %s <cantidad de iteraciones> <A> <TP> <PC> <TC>\n", prog_name);
	fprintf(stderr, "    0 < A < 1,\n");
	fprintf(stderr, "    0 < TP < 1,\n");
	fprintf(stderr, "    0 < PC < 1,\n");
	fprintf(stderr, "    0 < TC < 1,\n");
	fprintf(stderr, "    0 < EF < 1,\n");
	fprintf(stderr, "    0 < VE < 1,\n");
	exit(0);
}

string estadoFumador(T_estado_fumador ef) {
	string rsl = "";
	switch (ef)
	{
	case esperando: rsl = "esperando";
		break;
	case haciendo_cigarrillo: rsl = "haciendo cigarrillo";
		break;
	case fumando: rsl = "fumando";
		break;
	default:
		break;
	}
	return rsl;
}

/*
* Una version anterior basada en parallel sections no funciono porque tal estructura NO
* garantiza que en cada iteración cada hilo realiza siempre la misma seccion, lo cual 
* generaba estados inconsistentes en los fumadores.
*/