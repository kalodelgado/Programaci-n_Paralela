/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Laberinto.h
 * Author: Alan
 *
 * Created on 7 de febrero de 2018, 04:46 PM
 */

#ifndef LABERINTO_H
#define LABERINTO_H

#include <memory>
#include <iostream> //AGREGADO PARA USO DE: cout
using namespace std;

#include "Grafo.h"
#include "Adyacencia.h"
#include "Error_laberinto.h"

class Laberinto {
public:

    /* CONSTRUCTORES */

    // REQ: cantidadVtrs > 0. En este caso, N es igual a cantidadVrts.
    // REQ: 0 < probabilidadAdy < 1.
    // Construye un laberinto con cantidadVrts de vértices en el que el conjunto de
    // adyacencias se determina aleatoriamente utilizando probabilidadAdy. Esto
    // implica que la probabilidad de que un arco exista entre cualesquiera dos
    // vértices es igual a probabilidadAdy
    Laberinto(int cantidadVrts, double probabilidadAdy);

    // Construye un laberinto con base en los datos en el archivo.
    // El archivo debe ser de texto (extensión txt) con datos separados por comas.
    // En la primera línea aparecerá un entero que representa la cantidad N de vértices.
    // Los vértices tienen un índice que va de 0 a N-1.
    // Luego en cada línea aparecerán los índices de los vértices asociados o
    // adyacentes, a cada nodo: 0, 1...N-1.
    // NOTA: todos los vértices son inicializados en cero.
    Laberinto(ifstream& archivo);

    // Construye una copia idéntica a orig.    
    Laberinto(const Laberinto& orig);

    // destructor
    ~Laberinto();

    /* MÉTODOS OBSERVADORES BÁSICOS */

    // EFE: retorna true si 0 <= idVrt < N.
    // NOTA: idNdo significa "identificador de vértice".
    bool xstVrt(int idVrt) const;

    // REQ: 0 <= idVrtO < N && 0 <= idVrtD < N.
    // EFE: retorna true si existe adyacencia entre los vértices idVrtO e idVrtD.
    bool xstAdy(int idNdO, int idVrtD) const;

    int obtIdVrtInicial() const;
    
    int obtIdVrtFinal() const;
    
    // REQ: 0 <= idVrt < N.
    // EFE: retorna en "rsp" los identificadores idVrtj de todos los vértices 
    // adyacentes a idVrt en el grafo.
    void obtIdVrtAdys(int idVrt, vector< int >& rsp) const throw(Error_laberinto);

    // REQ: 0 <= idVrt1 < N && 0 <= idVrt2 < N
    // EFE: retorna los datos de la adyacencia entre <idVrtO, idVrtD>.
    // NOTA: retorna por valor para que NO pueda ser modificado.    
    Adyacencia obtDatoAdy(int idVrtO, int idVrtD) const throw(Error_laberinto);

    // EFE: retorna el total de arcos o adyacencias en el grafo.
    int obtTotAdy() const;

    // EFE: retorna el total de vértices en el grafo.
    int obtTotVrt() const;

    /* MÉTODOS OBSERVADORES NO BÁSICOS*/

    // REQ: 0 <= idVrt1 < N && 0 <= idVrt2 < N
    // EFE: retorna en "camino" los índices de los vértices que conforman el
    //      camino más corto entre idVrtO y idVrtD.
    //      retorna la longitud de este camino.
    // NOTA: se usa algoritmo de Dijkstra.
    int caminoMasCorto(int idVrtO, int idVrtD, vector< int >& camino) const throw(Error_laberinto);
    
    // REQ: 0 <= idVrt1 < N && 0 <= idVrt2 < N
    // EFE: retorna en "camino" los índices de los vértices que conforman el
    //      camino más corto entre idVrtO y idVrtD encontrado por la colonia de hormigas.
    //      retorna la longitud de este camino.    
    int caminoEncontrado(int idVrtO, int idVrtD, vector<int>& camino) const throw(Error_laberinto);
    
    // EFE: retorna la suma total de la ferormona de las adyacencias;
    double sumaTotalFerormona() const throw(Error_laberinto);

    /* MÉTODOS MODIFICADORES BÁSICOS */
    
    // REQ: 0 <= idVrtInicialN < N
    // EFE: asigna el identificador del vértice inicial del laberinto.
    void asgIdVrtInicial(int idVrtInicialN);
    
    // REQ: 0 <= idVrtFinalN < N
    // EFE: asigna el identificador del vértice inicial del laberinto.    
    void asgIdVrtFinal(int idVrtFinalN);

    // REQ: 0 <= idVrt1 < N && 0 <= idVrt2 < N
    // EFE: asigna el valor "ady" a la adyacencia <idVrtO, idVrtD>.
    void asgDatoAdy(int idVrtO, int idVrtD, const Adyacencia& ady) throw(Error_laberinto);
    
    /* OTROS MODIFICADORES */
    
    // EFE: decrementa la cantidad de ferormona de cada una de las adyacencias en decFerormona.
    void decrementarFerormonaAdys(double decrFerormona) throw(Error_laberinto);
    
    // EFE: actualiza la valoración de las adyacencias con base en la ferormona de cada una.
    void actualizarValoracionAdys() throw(Error_laberinto);
    
private:
    
    //Metodo recursivo que halla el camino siguiendo las adyacencias con mayor ferormona, pero devolviendose si se topa un camino sin salida
    void encontrarCamino(int idVrtD, vector<int>& camino) const;

    int idVrtInicial;
    int idVrtFinal;

    shared_ptr< Grafo< int, Adyacencia > > laberintoPtr;
    Grafo< int, Adyacencia >& laberinto; // para evitar notación de flecha
};

#endif /* LABERINTO_H */

