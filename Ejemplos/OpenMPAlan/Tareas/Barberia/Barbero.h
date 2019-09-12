#pragma once
#include <string>
using namespace std;

#include "Aleatorizador.h"
#include "Agente.h"
#include "ClienteBarbero.h"

class Barbero : public Agente
{
public:

	enum Estado {
		esperando,
		cortando,
		destruido
	};

	Barbero();
	Barbero(Barbero::Estado ne);
	virtual ~Barbero();

	virtual int estado();
	virtual void actuar();
	virtual string estadoString();

private:
	Estado e;
	shared_ptr< ClienteBarbero > clienteXatender_ptr;
};

Barbero::Barbero(): e (Estado::esperando), clienteXatender_ptr(0)
{
}

Barbero::Barbero(Estado ne): e(ne), clienteXatender_ptr(0)
{
}

Barbero::~Barbero()
{
}

int Barbero::estado()
{
	return e;
}

void Barbero::actuar()
{
	switch (e)
	{
		case esperando:
		{
			if (ClienteBarbero::clientesEsperando.size() > 0) {
				# pragma omp critical
				{
					clienteXatender_ptr = ClienteBarbero::clientesEsperando.front();
					ClienteBarbero& clienteXatender = *clienteXatender_ptr;
					ClienteBarbero::msjsClientes[clienteXatender.obtId()] = ClienteBarbero::siendoAtendido;
					ClienteBarbero::clientesEsperando.pop(); // pasa al siguiente cliente sacándolo de la cola
				}
				e = cortando;
			}
			break;
		}
		case cortando:
		{
			if (clienteXatender_ptr->obtDC() == 0)	// ya termino con el cliente
				e = esperando;
			break;
		}
		default: break;
	}
}

string Barbero::estadoString()
{
	string rsl = "";
	switch (e)
	{
	case esperando:
	{
		rsl = "esperando";
		break;
	}
	case cortando:
	{
		rsl = "cortando";
		break;
	}
	case destruido:
	{
		rsl = "destruido";
		break;
	}
	}
	return rsl;
}
