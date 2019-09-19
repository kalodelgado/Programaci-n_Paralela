#pragma once
#include <queue>
#include <unordered_map>
#include <memory>
#include <string>
using namespace std;

#include "Aleatorizador.h"
#include "Agente.h"

class ClienteBarbero : public Agente
{
public:

	enum Estado {
		esperando,
		siendoAtendido,
		destruido
	};
	
	static queue< shared_ptr< ClienteBarbero >> clientesEsperando;			// sillas de clientes esperando
	/* Mensajes del barbero a los clientes: K == id del cliente, V == mensaje con el nuevo estado que debe asumir el cliente. 
	*  Si el nuevo estado es igual a -1, indica que el cliente no encontró lugar en las sillas de espera de la barbería.
	*/
	static unordered_map< int, int > msjsClientes;	
	static int gnrdId;

	ClienteBarbero();
	ClienteBarbero(Estado ne, int ndc);
	virtual ~ClienteBarbero();

	int obtId();
	int obtDC();//duracion de cuanto será atendido, DuracionCliente
	virtual int estado();
	virtual void actuar();
	virtual string estadoString();

private:
	int id;
	Estado e;
	int dc;		// lo que todavía va a durar siendo atendido
};

queue< shared_ptr< ClienteBarbero >> ClienteBarbero::clientesEsperando;
unordered_map< int, int > ClienteBarbero::msjsClientes;
int ClienteBarbero::gnrdId = 0;

ClienteBarbero::ClienteBarbero(): id(ClienteBarbero::gnrdId), e (esperando), dc(5)
{
	gnrdId++;
}

ClienteBarbero::ClienteBarbero(Estado ne, int ndc): id(ClienteBarbero::gnrdId), e(ne), dc(ndc)
{
	gnrdId++;
}

ClienteBarbero::~ClienteBarbero()
{
}

int ClienteBarbero::obtId()
{
	return id;
}

int ClienteBarbero::obtDC()
{
	return dc;
}

int ClienteBarbero::estado()
{
	return e;
}

void ClienteBarbero::actuar()
{
	switch (e)
	{
		case esperando:
		{
			try { 
				int msj = msjsClientes.at(id);
				if (msj == -1)
					e = destruido;	// no pudo entrar a la cola porque estaba llena
				else e = Estado(msj);
				msjsClientes.erase(id); // se borra el mensaje porque ya lo procesó el cliente
			}
			catch(out_of_range oor) {
				// no tiene ningún mensaje del barbero ni de main => debe seguir esperando
			}
			break;
		}
		case siendoAtendido:
		{
			if (dc > 0)
				dc--;
			else e = destruido;
			break;
		}
		default: break;
	}
}

string ClienteBarbero::estadoString()
{
	string rsl = "";
	switch (e)
	{
	case esperando:
	{
		rsl = "esperando al barbero";
		break;
	}
	case siendoAtendido:
	{
		rsl = "siendo atendido";
		break;
	}
	case destruido:
	{
		rsl = "cliente barbero destruido";
		break;
	}
	}
	return rsl;
}
