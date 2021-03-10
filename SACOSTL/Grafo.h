/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Grafo.h
 * Author: Alan
 *
 * Created on 5 de febrero de 2018, 05:49 PM
 */

#ifndef GRAFO_H
#define GRAFO_H

#include <fstream>
#include <sstream>  //AGREGADO PARA USO DE: ostringstream
#include <vector>
#include <list>
#include <unordered_map>
#include <stack>
#include <algorithm>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
using namespace std;

#include "Error_grafo.h"

template < typename V, typename A >
class Grafo {
    // Representa un grafo con vértices de tipo V y adyacencias de tipo A.
    /* SUPUESTOS:
     * 1. Los vértices son identificados con números de 0 a N-1, dada N la cantidad
     * total de vértices.
     * 2. El tipo V tiene definido su constructor estándar V().
     * 3. El tipo V tiene definido su constructor de copias V(const V& orig);
     * 4. El tipo A tiene definido su constructor estándar A().
     * 5. El tipo A tiene definido su constructor de copias A(const A& orig);
     */

public:
    /* CONSTRUCTORES */

    // REQ: cantidadVrt > 0. En este caso, N es igual a cantidadVrt.
    // REQ: 0 < probabilidadAdy < 1.
    // Construye un grafo con cantidadVrt de vértices en el que el conjunto de
    // adyacencias se determina aleatoriamente utilizando probabilidadAdy. Esto
    // implica que la probabilidad de que un arco exista entre cualesquiera dos
    // dos vértices es igual a probabilidadAdy
    Grafo(int cantidadVrt, double probabilidadAdy);

    // Construye una red con base en los datos en el archivo.
    // El archivo debe ser de texto (extensión txt) con datos separados por comas.
    // En la primera línea aparecerá un entero que representa la cantidad N de vértices.
    // Los vértices tienen un índice que va de 0 a N-1.
    // Luego en cada línea aparecerán los índices de los vértices asociados o
    // adyacentes, a cada vértice: 0, 1...N-1.
    // NOTA: todos los vértices son inicializados con V().
    Grafo(ifstream& archivo);

    // Construye una copia idéntica a orig.
    Grafo(const Grafo< V, A >& orig);

    // Destructor
    ~Grafo();

    /* MÉTODOS OBSERVADORES BÁSICOS */

    // EFE: retorna true si 0 <= idVrt < N.
    // NOTA: idVrt significa "identificador de vértice".
    bool xstVrt(int idVrt) const;

    // REQ: 0 <= idVrtO < N && 0 <= idVrtD < N.
    // EFE: retorna true si existe adyacencia entre los vértices idVrtO e idVrtD.
    bool xstAdy(int idVrtO, int idVrtD) const;

    // REQ: 0 <= idVrt < N.
    // EFE: retorna en "rsp" los identificadores idVrtj de todos los vértices 
    // adyacentes a idVrt en el grafo.
    void obtIdVrtAdys(int idVrt, vector< int >& rsp) const throw (Error_grafo);

    // REQ: 0 <= idVrt < N.
    // EFE: retorna el vértice con índice idVrt.
    // NOTA: retorna por valor para que NO pueda ser modificado.
    V operator[](int idVrt) const throw (Error_grafo);

    // REQ: 0 <= idVrt1 < N && 0 <= idVrt2 < N
    // EFE: retorna los datos de la adyacencia entre <idVrtO, idVrtD>.
    // NOTA: retorna por valor para que NO pueda ser modificado.    
    A obtDatoAdy(int idVrtO, int idVrtD) const throw (Error_grafo);

    // EFE: retorna el total de arcos o adyacencias en el grafo.
    int obtTotAdy() const;

    // EFE: retorna el total de vértices en el grafo.
    int obtTotVrt() const;

    /* MÉTODOS OBSERVADORES NO BÁSICOS*/

    // REQ: 0 <= idVrt1 < N && 0 <= idVrt2 < N
    // EFE: retorna en "camino" los índices de los vértices que conforman el
    //      camino más corto entre idVrtO y idVrtD.
    int caminoMasCorto(int idVrtO, int idVrtD, vector< int >& camino) const throw (Error_grafo);

    /* MÉTODOS MODIFICADORES BÁSICOS */

    // REQ: 0 <= idVrt < N.
    // EFE: retorna el vértice con índice idVrt.
    // NOTA: retorna por referencia para que pueda ser modificado en el contexto
    // invocador.
    V& operator[](int idVrt) throw (Error_grafo);

    // REQ: 0 <= idVrt1 < N && 0 <= idVrt2 < N
    // EFE: asigna el valor "a" a la adyacencia <idVrtO, idVrtD>.
    void asgDatoAdy(int idVrtO, int idVrtD, const A& a) throw (Error_grafo);

private:

    template < typename W >
    struct Vrt {
        W w;
        list< int > lstAdy;

        Vrt< W >() : w(W()), lstAdy() {
        }; // constructor estándar de Vrt

        Vrt< W >(const W& ww) : w(ww), lstAdy() {
        }; // constructor con dato de vértice

        Vrt< W >(const Vrt< W >& vrt) : w(vrt.w), lstAdy(vrt.lstAdy) {
        }; // constructor de copias de Vrt
    };

    // REQ: (f >= 0) && (c >= 0) && (N > 0)
    // RET: valor único asociado a f y c con base en N = vectorVrts.size().

    static long obtIndiceAdy(int f, int c, int N) {
        if (f > c) {
            int t = f;
            f = c;
            c = t;
        }
        return f * N + c - f * (f + 1) / 2;
    };

    vector< Vrt< V > > vectorVrts; // vector de vértices
    unordered_map< long, A > mapAdys; // map basado en hashing de adyacencias

    bool buscarElemento(int i, int inicial) const {
        bool existe = false;
        for (typename list<int>::const_iterator iter = vectorVrts[i].lstAdy.begin(); iter != vectorVrts[i].lstAdy.end() && !existe; iter++) {
            if (*iter == inicial)
                existe = true;
        }
        return existe;
    };
};

template < typename V, typename A >
Grafo< V, A >::Grafo(int cantidadVrt, double probabilidadAdy) {
    int indice; //posicion en el arreglo de adyacencias
    double num; //numero que se igualara a un numero random

    vectorVrts.resize(cantidadVrt); //establece el tamaño inicial del vector
    mapAdys.reserve(cantidadVrt * (cantidadVrt + 1) / 2);
    srand(time(NULL));

    for (int i = 0; i < vectorVrts.size(); ++i) {
        for (int v = i + 1; v < vectorVrts.size() - 1; ++v) {
            num = rand() % 1000;
            num = num / 1000.0;
            if (num <= probabilidadAdy) {
                //genera el espejo
                vectorVrts[v].lstAdy.push_front(i);
                vectorVrts[i].lstAdy.push_front(v);
                indice = obtIndiceAdy(v, i, vectorVrts.size()); //se obtiene el indice de adyacencia para inicializar la feromona
                mapAdys.insert(std::pair<long, A>(indice, A()));
            }
        }
    }
}

template < typename V, typename A >
Grafo< V, A >::Grafo(ifstream& archivo) {
    int indice; // pocision en el arreglo de adyacencias
    int datoActual; //lee un numero del archivo
    char finLinea = ' ';
    int contVertice = 0; //nos dira el indice del vertice donde se debe agregar datoActual
    archivo >> datoActual;
    vectorVrts.resize(datoActual);
    mapAdys.reserve(datoActual * (datoActual + 1) / 2);

    archivo >> datoActual;
    archivo.get(); // consume un blanco
    finLinea = archivo.peek();

    while (!archivo.eof()) {
        while (!archivo.eof() && (finLinea != 10)) {
            vectorVrts[contVertice].lstAdy.push_back(datoActual); //se agrega el elemento a la lista
            indice = obtIndiceAdy(contVertice, datoActual, vectorVrts.size());
            mapAdys.insert(std::pair<long, A>(indice, A()));

            archivo >> datoActual;
            archivo.get(); // consume un blanco
            finLinea = archivo.peek();
        }
        if (!archivo.eof()) { //si sale del while por finLinea
            vectorVrts[contVertice].lstAdy.push_back(datoActual);
            indice = obtIndiceAdy(contVertice, datoActual, vectorVrts.size());
            mapAdys.insert(std::pair<long, A>(indice, A()));
        }
        archivo >> datoActual;
        archivo.get();
        finLinea = archivo.peek();
        int PRUEBA = vectorVrts[contVertice].lstAdy.size();
        ++contVertice; //se aumenta la posicion el vertice    
    }
}

template < typename V, typename A >
Grafo< V, A >::Grafo(const Grafo< V, A >& orig) {
    vectorVrts.resize(orig.vectorVrts.size());

    for (int i = 0; i < vectorVrts.size(); ++i) {
        vectorVrts[i].lstAdy = orig.vectorVrts[i].lstAdy;
    }

    for (typename unordered_map<long, A>::const_iterator iter = orig.mapAdys.begin(); iter != orig.mapAdys.end(); ++iter) {
        mapAdys.insert(std::pair<long, A>(iter->first, iter->second));
    }
}

template < typename V, typename A >
Grafo< V, A >::~Grafo() {
}

template < typename V, typename A >
bool Grafo< V, A >::xstVrt(int idVrt) const {
    bool cumple = false;

    if (0 <= idVrt && idVrt < vectorVrts.size()) {
        cumple = true;
    }

    return cumple;
}

template < typename V, typename A >
bool Grafo< V, A >::xstAdy(int idVrtO, int idVrtD) const {
    bool existe = false;
    int indice;
    if (xstVrt(idVrtO) && xstVrt(idVrtD)) {
        indice = obtIndiceAdy(idVrtO, idVrtD, vectorVrts.size());
        typename unordered_map<long, A>::const_iterator it;
        it = mapAdys.find(indice);
        if (it != mapAdys.end()) {
            existe = true;
        }
    }
    return existe;
}

template < typename V, typename A >
void Grafo< V, A >::obtIdVrtAdys(int idVrt, vector< int >& rspRetornar) const throw (Error_grafo) {
    if (xstVrt(idVrt)) {
        rspRetornar.clear();
        rspRetornar.resize(vectorVrts.at(idVrt).lstAdy.size());
        std::copy(vectorVrts.at(idVrt).lstAdy.begin(), vectorVrts.at(idVrt).lstAdy.end(), rspRetornar.begin());
    } else {
        ostringstream errorSalida;
        errorSalida << idVrt;
        throw Error_grafo(1, errorSalida.str());
    }
}

template < typename V, typename A >
V Grafo< V, A >::operator[](int idVrt) const throw (Error_grafo) {
    if (xstVrt(idVrt)) {
        return vectorVrts.at(idVrt).w;
    } else {
        ostringstream errorSalida;
        errorSalida << idVrt;
        throw Error_grafo(1, errorSalida.str());
    }
}

template < typename V, typename A >
A Grafo< V, A >::obtDatoAdy(int idVrtO, int idVrtD) const throw (Error_grafo) {
    if (xstAdy(idVrtO, idVrtD)) {
        int indice = obtIndiceAdy(idVrtO, idVrtD, vectorVrts.size());
        return mapAdys.at(indice); //retorna los datos de adyacencia para esa "llave"
    } else {
        ostringstream errorSalida;
        errorSalida << idVrtO << ", " << idVrtD;
        throw Error_grafo(2, errorSalida.str());
    }
}

template < typename V, typename A >
int Grafo< V, A >::obtTotAdy() const {
    int contAdyacencia = 0;
    //se suman todas las adyacencias de los vertices
    for (int i = 0; i < vectorVrts.size(); ++i) {
        contAdyacencia += vectorVrts[i].lstAdy.size();
    }
    return contAdyacencia;
}

template < typename V, typename A >
int Grafo< V, A >::obtTotVrt() const {
    return vectorVrts.size();
}

template < typename V, typename A >
V& Grafo< V, A >::operator[](int idVrt) throw (Error_grafo) {
    if (xstVrt(idVrt)) {
        return &vectorVrts.at(idVrt).w;
    } else {
        ostringstream errorSalida;
        errorSalida << idVrt;
        throw Error_grafo(1, errorSalida.str());
    }
}

template < typename V, typename A >
int Grafo< V, A >::caminoMasCorto(int idVrtO, int idVrtD, vector< int >& camino) const throw (Error_grafo) {

    bool rsl = false;
    bool tieneCamino = true;
    int ultimo;
    int inicial;
    std::vector< int > distancia;

    if (xstVrt(idVrtO) && xstVrt(idVrtD)) {
        // Basado en algoritmo de Dijkstra
        const int imax = 2147483647;
        inicial = idVrtO;
        ultimo = idVrtD;     
        distancia.resize(vectorVrts.size());
        std::vector< int > visto;
        visto.resize(vectorVrts.size()); //vector que nos dice si ya el numero ha sido tomado en cuenta
        //int visto[cntVrts] = {0};
        std::vector< int > anterior;
        anterior.resize(vectorVrts.size()); //vector con el camino mas corto
        for (int i = 0; i < anterior.size(); ++i) {
            visto[i] = 0;
            anterior[i] = -1;
        }
        int vertice;
        std::vector< int > vecinos; //creamos un vector que almacenara las adyacencias de un indice
        int totalVecinos;

        for (int i = 0; i < distancia.size(); ++i) {
            if (buscarElemento(i, inicial)) {
                distancia[i] = 1;
                anterior[i] = inicial;
            } else {
                distancia[i] = imax;
            }
        }

        distancia[inicial] = 0;
        visto[inicial] = 1;

        while (!visto[ultimo] && tieneCamino) {//mientras no se haya llegado al ultimo

            int valorTemp = imax; //asigna el mayor valor al inicio para que siempre pueda ser cambiado
            for (int i = 0; i < distancia.size(); ++i) {
                if (distancia[i] < valorTemp && visto[i] != 1) {
                    valorTemp = distancia[i];
                    vertice = i;
                }
            }


            if (valorTemp != imax) {//Si se ha encontrado un vecino proximo que aun no haya sido visto ----- SI NO SE ENCUENTRA, QUIERE DECIR QUE NO EXISTE CAMINO

                visto[vertice] = 1; //visto[vertice] = cierto; se ha revisado el vertice actual

                std::vector<int> vecinos;
                obtIdVrtAdys(vertice, vecinos); //llama este metodo porque este tambien modifica al vector que recibe

                totalVecinos = vecinos.size();

                for (int i = 0; i < totalVecinos; ++i) {
                    if (anterior[vecinos[i]] == -1) {//si aun no tiene un anterior, lo sustituye por vertice
                        anterior[vecinos[i]] = vertice;
                    }
                    if (distancia[vecinos[i]] > distancia[vertice] + 1) {
                        distancia[vecinos[i]] = distancia[vertice] + 1;
                    }
                }
            } else {
                tieneCamino = false;
            }
        }

        if (tieneCamino) {

            rsl = true; //Tiene un camino entre el inicio y el final

            int cont = 0;
            int acomoda = ultimo;

            camino.resize(distancia.size() + 1);

            camino[cont] = ultimo;
            ++cont;
            while (acomoda != inicial) {
                camino[cont] = anterior[acomoda];
                acomoda = anterior[acomoda];
                ++cont;
            }
        }
    }


    if (rsl) {
        return distancia[ultimo];
    } else {
        ostringstream errorSalida;
        errorSalida << idVrtO << ", " << idVrtD;
        throw Error_grafo(3, errorSalida.str());
    }
}

template < typename V, typename A >
void Grafo< V, A >::asgDatoAdy(int idVrtO, int idVrtD, const A& a) throw (Error_grafo) {
    if (xstAdy(idVrtO, idVrtD)) {
        int indice = obtIndiceAdy(idVrtO, idVrtD, vectorVrts.size());
        mapAdys[indice] = a;
    } else {
        ostringstream errorSalida;
        errorSalida << idVrtO << ", " << idVrtD;
        throw Error_grafo(2, errorSalida.str());
    }
}
#endif /* GRAFO_H */

