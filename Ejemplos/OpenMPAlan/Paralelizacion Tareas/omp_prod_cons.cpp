/* 
* 
* Propósito:  Resolver el ejercicio corto #4 para OpenMP.
*
* Compilación:  g++ -g -Wall -fopenmp -o omp_msglk omp_adm_msj.cpp queue_lk.h
*
* Entradas:    
*			cantidad de hilos productores y consumidores
*			archivos que leerán los productores, deben tener el nombre con forma "arch*.txt", por ejemplo: "arch0.txt" o "arch1.txt
* "Salida:   Por consola, los hilos consumidores desplegarán las líneas "tokenizadas" de los archivos.
*
*
* Notes:
* 1.  DEBUG == 1 genera salida más detallada para depuración
* 2.  Esta es la versión básica que usa una cola.
*
*/

#define _CRT_SECURE_NO_WARNINGS 1
#define DEBUG 1 // activa código de depuración

#include <string>
#include <omp.h>
#include <iostream>
#include <vector>
#include <queue>
#include <fstream> // para ifstream
#include <sstream> // stringstream
#include <iterator> // para istream_iterator
using namespace std;

const int MAX_MSG = 10000;

/* Tipo de mensajes con contenido TT == T */
template < typename TT >
struct msg_t {
	int src_rank; // id del hilo fuente
	TT msg; // mensaje
			/* constructores */
	msg_t() : src_rank(0), msg(TT()) {};
	//msg_t(int sr) : src_rank(sr), msg() {}; genera C2535 member function already defined or declared cuando TT == int
	msg_t(TT m) : src_rank(0), msg(m) {};
	msg_t(int sr, const TT& tt) : src_rank(sr), msg(tt) {};
};

bool validaCntHilos(int ch);
void send_msg(queue< msg_t< std::string > >& msg_queue, int my_rank, std::string mesg);
bool try_receive(queue< msg_t< std::string > >& q_p, msg_t< std::string >& msg);
bool done(queue< msg_t< std::string > >& q_p, int done_sending, int pthread_count);

int main(int argc, char* argv[]) {
	int cthread_count = 0;
	int pthread_count = 0;
	int thread_count = 0;
	int done_sending = 0;
	queue< msg_t< std::string > > msg_queue; // cola de mensajes de línea
	
	// declaración del tipo de roles y vector
	enum Rol { consumidor, productor };
	vector< Rol > roles;


	while (!validaCntHilos(cthread_count) || !validaCntHilos(pthread_count)) {
		cout << "Digite la cantidad de hilos consumidores ( >= 1 ): ";
		cin >> cthread_count;
		cout << "Digite la cantidad de hilos productores ( >= 1 ): ";
		cin >> pthread_count;
	}

	// se crea e inicializa el vector de roles de los hilos:
	thread_count = cthread_count + pthread_count;
	roles.resize(thread_count);
	for (int i = 0; i < pthread_count; i++)
		roles[i] = productor;
	for (int i = 0; i < cthread_count; i++)
		roles[i + pthread_count] = consumidor;

#  pragma omp parallel num_threads(thread_count) shared(msg_queue, roles, done_sending, pthread_count)
	{
		int ThreadId = omp_get_thread_num();

		if (roles[ThreadId] == productor) { // como productor le toca leer su archivo por líneas y poner los mensajes en la cola
			std::string linea;
			stringstream nombreArchStream;
			nombreArchStream << "arch" << ThreadId << ".txt";
			std::string nombreArchivo = nombreArchStream.str();
			ifstream archivo(nombreArchivo, ios::in);
			if (!archivo) { // operador ! sobrecargado
				cout << "No se pudo abrir el archivo de entrada: " << nombreArchivo << endl;
			}
			else { // lee el archivo y genera los mensajes
				getline(archivo, linea);
				while (linea != "") {
					send_msg(msg_queue, ThreadId, linea);
					getline(archivo, linea);
				}
				#pragma omp atomic
				done_sending++;
			}
		}
		else { // como consumidor le toca leer mensajes de la cola, tokenizarlos y desplegar las palabras por la consola
			msg_t< std::string > msg;
			bool sigue = try_receive(msg_queue, msg);

			while (sigue || !done(msg_queue, done_sending, pthread_count)) {
				// tokeniza la línea leída, si logró leer un mensaje, de lo contrario sigue intentándolo
				if (msg.msg != "") {
					std::istringstream iss(msg.msg);
					int enviador = msg.src_rank;
					// encontrado en: https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/ 
					std::vector<std::string> palabras((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());//Se para las álabras de la linea y las almacena en un vec
					# pragma omp critical
					{ // es critical por acceso a cout
						cout << "C:" << ThreadId << "P:" << enviador << "-->";
						for (int i = 0; i < palabras.size(); i++)
							cout << palabras[i] << "/";
						cout << endl;
						//cin.ignore();
					}
				}
				sigue = try_receive(msg_queue, msg); // intenta leer otra vez
			}
		}
	}

	// para evitar que se cierre la consola en Visual Studio.
	cin.ignore(); cin.ignore();
	return 0;
}

bool validaCntHilos(int ch) {
	return ch >= 1;
}

// ct es la cantidad de términos de la serie
bool validaCntMensajes(int cm) {
	return cm >= 1;
}

/*-------------------------------------------------------------------*/
void send_msg(queue< msg_t< std::string > >& msg_queue, int my_rank, string mesg) {
	//omp_set_lock(&lock);
	msg_t<string> n_msg(my_rank, mesg);
	# pragma omp critical
	msg_queue.push(n_msg);
	//omp_unset_lock(&lock);

#  ifdef DEBUG
# pragma omp critical
	cout << "Thread > " << my_rank << " sent " << mesg << endl;
#  endif
}  /* send_msg */

/*-------------------------------------------------------------------*/
bool try_receive(queue< msg_t< string > >& q_p, msg_t< string >& msg) {
	bool rsl = false;
	//omp_set_lock(&lock);
	# pragma omp critical
	if (!q_p.empty()) {
			msg = q_p.front();
			q_p.pop();
			rsl = true;
		}
	//omp_unset_lock(&lock);
	return rsl;
}   /* try_receive */

/*-------------------------------------------------------------------*/
bool done(queue< msg_t< string > >& q_p, int done_sending, int pthread_count) {
	if (q_p.size() == 0 && done_sending == pthread_count)
		return true;
	else
		return false;
}   /* done */