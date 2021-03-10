/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Hormiga.h
 * Author: Alan
 *
 * Created on 7 de febrero de 2018, 05:53 PM
 */

#ifndef HORMIGA_H
#define HORMIGA_H

#include <stdlib.h>     /* para srand, rand al moverse*/
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#include "Laberinto.h"
#include "Error_laberinto.h"

class Hormiga {
public:

    // EFE: construye un hormiga en el nido y con destino == nodoFinal.
    Hormiga();

    // EFE: construye una copia identica e independiente de orig.
    Hormiga(const Hormiga& orig);

    virtual ~Hormiga();

    /* METODOS OBSERVADORES */

    // EFE: retorna true si la hormiga ya salio del vertice inicial.
    bool salio();

    // EFE: retorna true si la hormiga ya regresa al vertice inicial despues de encontrar el vertice final.
    bool regreso();

    // EFE: retorna 'F' o 'I' para indicar el destino de la hormiga.
    char obtDestino();

    // EFE: retorna una hilera con la memoria de la hormiga.
    // EJEMPLO: "[2,6,7,12,54]" indicarÃƒÂ­a que la hormiga saliÃƒÂ³ por 2 y viajÃƒÂ³ en
    // el orden de la secuencia hasta 54.
    string obtMemoria();

    /* METODOS MODIFICADORES */

    // EFE: activa la hormiga y la ubica en nodoInicial.
    void salir();

    // REQ: que la hormiga (*this) esta activa.
    // EFE: la hormiga avanza a un vertice adyacente.
    void mover() throw(Error_laberinto);

    // EFE: asigna la referencia al laberinto que accesarán todas las hormigas.
    static void asgLaberinto(Laberinto& lbrt);

private:
    
    /* ATRIBUTOS ESTÁTICOS PRIVADOS ACCESIBLES POR TODAS LAS HORMIGAS */
    static Laberinto* laberinto_p; // puntero a laberinto 

    /* METODOS PRIVADOS SUGERIDOS */

    // EFE: retorna -1 en caso de que la hormiga llegue a camino sin salida,
    // y un idVrt vAlido en caso de que la hormiga pueda continuar.
    int seleccionaAdyMasCargada() throw(Error_laberinto);
        
    /* METODOS AUXILIARES */
    // REQ: que los idVrt en vrtsPosibles sean indices validos de vertices en el laberinto que esta recorriendo la hormiga.
    // EFE: elimina de vrtsPosibles los idVrt que ya aparecen en la memoria.
    void filtraVrtsPosibles(vector<int>&);
    
    bool estaEnMemoria(int);
    
    bool laberintoAsignado();
    
    /* ATRIBUTOS PRIVADOS OBLIGATORIOS */
    vector<int> memoria; // vector de idVrt que representa el recorrido de la hormiga
    int idVrtActual; // idVrt del vértice donde se encuentra la hormiga actualmente    
    bool haSalido; // false mientras no haya salido, true si ya salió
    bool haRegresado; // false mientras no haya regresado de idVrtFinal a idVrtInicial
    char destino; // 'F' o 'I', indica hacia adónde se dirige la hormiga
    int enRetroceso; // si es mayor a cero, la hormiga está retrocediendo esta cantidad de pasos porque topó con un camino sin salida o un ciclo-
    int longitudSolucion; // registra la longitud de la solución encontrada para moderar su aporte de ferormona en su retorno
    double deltaFerormona; // representa cuánta ferormona aporta la hormiga a cada link y se calcula como 1 / longitudSolucion.
};
#endif /* HORMIGA_H */





