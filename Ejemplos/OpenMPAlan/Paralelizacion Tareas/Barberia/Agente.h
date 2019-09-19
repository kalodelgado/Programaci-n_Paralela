#pragma once
#include <string>
using namespace std;

#include "Aleatorizador.h"

class Agente
{
public:

	enum Estado {
		esperando,
		activo,
		destruido
	};

	Agente();
	Agente(Estado ne, double npce);
	virtual ~Agente();

	virtual int estado();
	virtual void actuar();
	virtual string estadoString();

private:
	Estado e;
	double pce;		// probabilidad de cambio de estado
};

Agente::Agente(): e (Estado::esperando), pce(0)
{
}

Agente::Agente(Estado ne, double npce): e(ne), pce(npce)
{
}

Agente::~Agente()
{
}

int Agente::estado()
{
	return e;
}

void Agente::actuar()
{
	if (Aleatorizador::random_uniform_real(Aleatorizador::generador) < pce)
		switch (e)
		{
		case esperando:
		{
			e = activo;
			break;
		}
		case activo:
		{
			e = destruido;
			break;
		}
		default: break;
		}
}

string Agente::estadoString()
{
	string rsl = "";
	switch (e)
	{
	case esperando:
	{
		rsl = "esperando";
		break;
	}
	case activo:
	{
		rsl = "activo";
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
