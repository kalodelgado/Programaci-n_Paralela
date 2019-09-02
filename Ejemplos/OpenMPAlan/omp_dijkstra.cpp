//****************************************************************************80
//
//  Purpose:
//
//    MAIN runs an example of Dijkstra's minimum distance algorithm.
//
//  Discussion:
//
//    Given the distance matrix that defines a graph, we seek a list
//    of the minimum distances between node 0 and all other nodes.
//
//    This program sets up a small example problem and solves it.
//
//    The correct minimum distances are:
//
//      0   35   15   45   49   41
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    02 July 2010
//
//  Author:
//
//    Original C version by Norm Matloff, CS Dept, UC Davis.
//    Original C++ version by John Burkardt.
//	  This full C++ (spanish) version by Alan Calder�n Castro, ECCI, UCR, Costa Rica.

# include <cstdlib>
# include <iomanip>
# include <ctime>
# include <omp.h>
# include <iostream>
#include <vector>
using namespace std;
#define _CRT_SECURE_NO_WARNINGS 0 // para deshabilitar errores por uso de funciones deprecated

# define NV 6
# define i4_huge 2147483647

typedef vector< vector< int > > T_vec_vec_int;
typedef vector< int > T_vec_int;

void distancias_dijkstra(const T_vec_vec_int& ohd, T_vec_int& mind);
void buscar_mas_cercano(int s, int e, const T_vec_int& mind, const vector< bool >& connected, int& d, int& v);
void inicializar(T_vec_vec_int& ohd);
void timestamp(void);
void actualizar_mind(int s, int e, int mv, const vector< bool > connected, const T_vec_vec_int ohd, T_vec_int& mind);

int main(int argc, char **argv) {
	T_vec_int mind; // vector con distancias m�nimas a partir del nodo cero
	mind.resize(NV, 0);
	T_vec_vec_int ohd;
	ohd.resize(NV, mind); // matriz con distancias reales entre cada dos nodos o matriz de adyacencias

	//timestamp(); est� provocando un error en tiempo de ejecuci�n

	cout << "INICIA DIJKSTRA_OPENMP" << endl;
	
	// se inicializa la matriz de adyacencias:
	inicializar(ohd);

	// se imprime la matriz de adyacencias:
	for (int i = 0; i < NV; i++) {
		for (int j = 0; j < NV; j++) {
			if (ohd[i][j] == i4_huge)
			{
				cout << "  Inf";
			}
			else
			{
				cout << "  " << setw(3) << ohd[i][j];
			}
		}
		cout << endl;
	}

	// ejecuci�n concurrente del algoritmo de Dijkstra:
	distancias_dijkstra(ohd, mind);

	// impresi�n de resultados:
	cout << "  Distancias m�nimas a partir del nodo 0:" << endl;
	for (int i = 0; i < NV; i++)
	{
		cout << "  " << setw(2) << i
			<< "  " << setw(2) << mind[i] << "\n";
	}

	// finalizaci�n:
	cout << endl;
	cout << "TERMINA DIJKSTRA_OPENMP" << endl;
	// timestamp();

	// para que no se cierre la consola inmediatamente:
	int int_falso;
	cin >> int_falso;
	return 0;
}

// REQ: ohd haya sido correctamente inicializada con los datos de la matriz de adyacencias.
// MOD: N/A.
// EFE: retorna por referencia en "mind" las distancias de los caminos m�s cortos entre 0 y todos los dem�s nodos. 
void distancias_dijkstra(const T_vec_vec_int& ohd, T_vec_int& mind) {
	vector< bool > connected;
	int md;  // m�nimo global encontrado en una iteraci�n
	int mv;  // �ndice del m�nimo global encontrado en una iteraci�n
	int my_first; // inicio de iteraci�n por hilo
	int my_id; // identificador de hilo
	int my_last; // fin de iteraci�n por hilo
	int my_md; // m�nimo local por hilo
	int my_mv; // �ndice del m�nimo local por hilo
	int nth; // cantidad total de hilos

	//  Iniciar con el nodo 0 conectado al �rbol de caminos.
	connected.resize(NV,false);
	connected[0] = true;

	//  Inicializar la distancia m�nima desde el nodo cero a sus adyacencias pr�ximas.
	mind.resize(NV,0);
	for (int i = 0; i < NV; i++)
	{
		mind[i] = ohd[0][i];
	}

	//
	//  Se inicia la regi�n paralela: inicia la carrera de hilos.
	//
	# pragma omp parallel private ( my_first, my_id, my_last, my_md, my_mv ) shared ( connected, md, mind, mv, nth, ohd ) 
	{
		my_id = omp_get_thread_num(); // se obtiene el identificador del hilo actual
		nth = omp_get_num_threads();  // se obtiene el total de hilos disponibles o trabajadores del equipo.
		my_first = (my_id       * NV) / nth; // se calcula el l�mite inferior de las iteraciones que realizar� cada hilo
		my_last = ((my_id + 1) * NV) / nth - 1; // se calcula el l�mite superior de las iteraciones que realizar� cada hilo

		//  Directiva SINGLE: el bloque de c�digo s�lo es ejecutado por un hilo, el primero en llegar.
		# pragma omp single 
		{
			cout << endl;
			cout << "  P" << my_id
				<< ": La region paralela comienza con " << nth << " hilos." << endl;
			cout << endl;
		}

		cout << "  P" << my_id
			<< ":  Primero=" << my_first
			<< "  Ultimo=" << my_last << endl;

		//  Agrega (al �rbol de caminos) un nodo m�s con cada iteraci�n.
		for (int my_step = 1; my_step < NV; my_step++)
		{
			//  Cada vez que se inicia la b�squeda del siguiente nodo m�s cercano se inicializa 
			//  MD con un valor alto. S�lo un hilo necesita hacer esto.
			# pragma omp single 
			{
				md = i4_huge;
				mv = -1;
			}

			//  Cada hilo encuentra el m�s cercano nodo no conectado en la parte del grafo asignada.
			//  Algunos hilos no tendr�n ning�n nodo no conectado
			buscar_mas_cercano(my_first, my_last, mind, connected, my_md, my_mv);

			//  CRITICAL: para determinar el nodo m�s cercano global s�lo un hilo a la vez 
			//  debe ejecutar este bloque.
			# pragma omp critical
			{
				if (my_md < md)
				{
					md = my_md;
					mv = my_mv;
				}
			}

			// Esta barrera sincronizadora garantiza que ning�n hilo continue hasta tanto TODOS hayan llegado aqu�.
			# pragma omp barrier

			//
			//  Si MV es -1, entonces NING�N nodo encontr� un nodo desconectado, se termina antes de encontrar todas las distancias. 
			//  Si MV es -1, no se puede usar "break" dentro de una secci�n parallel de openmp, por tanto se debe continuar evitando m�s actualizaciones de mind.
			//  Si MV es diferente de -1, se debe conectar al siguiente m�s cercano de los nodos.
			//
			# pragma omp single 
			{
				if (mv != -1)
				{
					connected[mv] = true;
					cout << "  P" << my_id
						<< ": Conectando al nodo " << mv << endl;
				}
			}

			// se necesita otra barrera para evitar que alg�n hilo continue sin que se haya conectado el nodo siguiente m�s cercano.
			# pragma omp barrier

			// Ahora cada hilo debe actualizar su porci�n del vector MIND chequeando si hay una distancia m�s corta pasando por el 
			// siguiente nodo m�s cercano cuyo �ndice es MV.
			if (mv != -1)
			{
				actualizar_mind(my_first, my_last, mv, connected, ohd, mind);
			}

			// Antes de comenzar la siguiente iteraci�n se ocupa que todos los hilos hayan actualizado su porci�n del vector MIND.
			#pragma omp barrier
		}

		// Una vez que todos los nodos hayan sido conectados al �rbol, se termina el proceso.
		# pragma omp single
		{
			cout << endl;
			cout << "  P" << my_id << ": Saliendo de la region paralela" << endl;
		}
	}
}

// REQ: s <= e y que mind y connected hayan sido correctamente inicializados.
// MOD: d y v
// EFE: retorna por referencia en d el valor mind[k], o sea la distancia al nodo k m�s cercano no conectado a�n al �rbol
//		con s <= k <= e.
void buscar_mas_cercano(int s, int e, const T_vec_int& mind, const vector< bool >& connected, int& d, int& v) {
	d = i4_huge;
	v = -1;
	for (int i = s; i <= e; i++)
	{
		if (!connected[i] && mind[i] < d)
		{
			d = mind[i];
			v = i;
		}
	}
}

// REQ: que ohd tenga el tama�o correcto (NV x NV).
// MOD: ohd.
// EFE: inicializa ohd con las distancias entre los nodos 0 a 5 de acuerdo con el siguiente diagrama del grafo:
//    N0--15--N2-100--N3           0   40   15  Inf  Inf  Inf
//      \      |     /            40    0   20   10   25    6
//       \     |    /             15   20    0  100  Inf  Inf
//        40  20  10             Inf   10  100    0  Inf  Inf
//          \  |  /              Inf   25  Inf  Inf    0    8
//           \ | /               Inf    6  Inf  Inf    8    0
//            N1
//            / \
//           /   \
//          6    25
//         /       \
//        /         \
//      N5----8-----N4
void inicializar(T_vec_vec_int& ohd) {
	for (int i = 0; i < NV; i++)
	{
		for (int j = 0; j < NV; j++)
		{
			if (i == j)
			{
				ohd[i][i] = 0;
			}
			else
			{
				ohd[i][j] = i4_huge;
			}
		}
	}

	ohd[0][1] = ohd[1][0] = 40;
	ohd[0][2] = ohd[2][0] = 15;
	ohd[1][2] = ohd[2][1] = 20;
	ohd[1][3] = ohd[3][1] = 10;
	ohd[1][4] = ohd[4][1] = 25;
	ohd[2][3] = ohd[3][2] = 100;
	ohd[1][5] = ohd[5][1] = 6;
	ohd[4][5] = ohd[5][4] = 8;
}

// EFE: imprime una marca de tiempo.
void timestamp(void) {
# define TIME_SIZE 40

	static char time_buffer[TIME_SIZE];
	struct std::tm *tm_ptr = 0;
	size_t len;
	std::time_t now;

	now = std::time(NULL);
	localtime_s( tm_ptr, &now);

	len = std::strftime(time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm_ptr);

	std::cout << time_buffer << endl;
}

// REQ: s <= e y connected, ohd y mind hayan sido correctamente inicializados.
// MOD: mind.
// EFE: actualiza el vector mind con las menores distancias del nodo 0 a todos los dem�s pasando por el nodo mv.
void actualizar_mind(int s, int e, int mv, const vector< bool > connected, const T_vec_vec_int ohd, T_vec_int& mind) {
	for (int i = s; i <= e; i++) {
		if (!connected[i]) {
			if (ohd[mv][i] < i4_huge) {
				if (mind[mv] + ohd[mv][i] < mind[i]) {
					mind[i] = mind[mv] + ohd[mv][i];
				}
			}
		}
	}
}