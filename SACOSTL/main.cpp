/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: alan.calderon
 *
 * Created on 2 de abril de 2018, 10:31 AM
 */

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <vector>
using namespace std;

#include "Laberinto.h"
#include "Simulador.h"

/*
 * 
 */
int main(int argc, char** argv) {

    int cntVrt = 0, cntHor = 0, idInicial = 0, idFinal = 0, p = 0, salir = 0;
    double probAd, decFer = 0.0;
    std::vector<int> camino;

    string nombreArchivo;
    bool noesValido = false;
    bool continua = true;
    int op;

    while (continua) {

        cout << "\n\n*************SIMULACION DE SACO*************\n\n";
        cout << "Elija una opcion \n Crear un Laberinto mediante: \n 1) Proporcionando parametros \n 2) Por medio de un archivo \n 0) Salir \n" << endl;
        cin >> op;
        switch (op) {
            case 1:
                do {
                    cout << "\nDigite la cantidad de vertices que tendra el laberinto" << endl;
                    cin >> cntVrt;
                    if (cntVrt <= 0)
                        cerr << "Digito una cantidad nula \n" << endl;
                } while (cntVrt <= 0);

                do {
                    cout << "\nDigite el vertice inicial \n" << endl;
                    cin >> idInicial;
                    if (idInicial < 0 || idInicial > cntVrt)
                        cerr << "Numero de vertice no valido \n" << endl;
                } while (idInicial < 0 || idInicial > cntVrt);

                do {
                    cout << "\nDigite el vertice final \n" << endl;
                    cin >> idFinal;
                    if (idFinal < 0 || idFinal > cntVrt || idFinal == idInicial)
                        cerr << "Número de vertice no valido \n" << endl;
                } while (idFinal < 0 || idFinal > cntVrt || idFinal == idInicial);

                do {
                    cout << "\nDigite la probabilidad de que exista la adyacencia entre los vertices \n Procure que la probabilidad este entre 0 y 1 \n" << endl;
                    cin >> probAd;
                    if (probAd >= 1 || probAd < 0)
                        cerr << "Probabilidad no valida \n" << endl;
                } while (probAd >= 1 || probAd < 0);

                do {
                    cout << "\nDigite la cantidad de hormigas \n" << endl;
                    cin >> cntHor;
                    if (cntHor <= 0)
                        cerr << "Digito una cantidad nula \n" << endl;
                } while (cntHor <= 0);

                do {
                    cout << "\nDigite cuanto se decrementara la ferormona \n Procure que este entre 0 y 1 \n" << endl;
                    cin >> decFer;
                    if (decFer >= 1 || decFer < 0)
                        cerr << "El valor no cumple los requisitos \n" << endl;
                } while (decFer >= 1 || decFer < 0);

                do {
                    cout << "\nDigite la cantidad de pasos de simulacion (total de tics) \n Estos deberan ser mayores que 1 \n" << endl;
                    cin >> p;
                    if (p < 1)
                        cerr << "La cantidad de tics no es valida \n" << endl;
                } while (p < 1);

                break;

            case 2:

                do {
                    cout << "\nDigite el nombre del archivo seguido de .txt \n" << endl;
                    cin >> nombreArchivo;
                    ifstream archivo(nombreArchivo, ios::in);
                    if (!archivo) { // operador ! sobrecargado
                        noesValido = true;
                        cerr << "Has digitado mal el nombre del archivo o este no es valido \n" << endl;
                    } else {
                        noesValido = false;
                        archivo >> cntVrt;
                    }
                } while (noesValido);

                do {
                    cout << "\nDigite el vertice inicial \n" << endl;
                    cin >> idInicial;
                    if (idInicial < 0 || idInicial > cntVrt)
                        cerr << "Numero de vertice no valido \n" << endl;
                } while (idInicial < 0 || idInicial > cntVrt);

                do {
                    cout << "\nDigite el vertice final \n" << endl;
                    cin >> idFinal;
                    if (idFinal < 0 || idFinal > cntVrt || idFinal == idInicial)
                        cerr << "Número de vertice no valido \n" << endl;
                } while (idFinal < 0 || idFinal > cntVrt || idFinal == idInicial);

                do {
                    cout << "\nDigite la cantidad de hormigas \n" << endl;
                    cin >> cntHor;
                    if (cntHor <= 0)
                        cerr << "Digito una cantidad nula \n" << endl;
                } while (cntHor <= 0);

                do {
                    cout << "\nDigite cuanto se decrementara la ferormona \n Procure que este entre 0 y 1 \n" << endl;
                    cin >> decFer;
                    if (decFer >= 1 || decFer < 0)
                        cerr << "El valor no cumple los requisitos \n" << endl;
                } while (decFer >= 1 || decFer < 0);

                do {
                    cout << "\nDigite la cantidad de pasos de simulacion (total de tics) \n Estos deberan ser mayores que 1 \n" << endl;
                    cin >> p;
                    if (p < 1)
                        cerr << "La cantidad de tics no es valida \n" << endl;
                } while (p < 1);
                break;

            default:
                if (op == 0)
                    continua = false;
                else
                    cerr << "ERROR Debe digitar un numero valido (1,2,0) \n" << endl;
        }

        if (op == 1) {
            Laberinto laberinto(cntVrt, probAd);
            laberinto.asgIdVrtInicial(idInicial);
            laberinto.asgIdVrtFinal(idFinal);

            Simulador simulador(laberinto);

            simulador.iniciar(idInicial, idFinal, cntHor, decFer);

            try {
                simulador.ejecutar(p);
                cout << "Distancia con Dijkstra: " << laberinto.caminoMasCorto(idInicial, idFinal, camino) << "\n" << endl;

                stringstream fs; //construye una instancia de flujo de salida
                std::vector< int >::iterator ite;

                camino.clear();

                int p = laberinto.caminoEncontrado(idInicial, idFinal, camino);
                cout << "Distancia con SACO: " << p << "\n" << endl;

                if (!p) {
                    cout << "\nEl programa ha fallado en caminoEncontrado, todas las adyacencias poseen 0 de ferormona. Esto se debe a un error desconocido ajeno a la logica del codigo" << endl;
                    cout << "El fallo no ocurre siempre por lo que se puede ejecutar nuevamente el programa." << endl;
                    cout << "Si se depura el programa, y se entra manualmente en Simulador -> ejecutar() -> for() encargado de controlar los tics de reloj : " << endl;
                    cout << "Se puede utilizar el arreglo (double ady[18], adaptado para laberintop.txt) y ejecutar algunas iteraciones de forma manual para comprobar que las adyacencias se rellenan correctamente" << endl;
                }
            } catch (Error_Simulador e) {
                cout << e.what() << endl;
            }

        } else if (op == 2) {

            ifstream archivo(nombreArchivo, ios::in);
            Laberinto laberintoArch(archivo);

            Simulador simulador(laberintoArch);
            simulador.iniciar(idInicial, idFinal, cntHor, decFer);

            try {
                simulador.ejecutar(p);

                cout << "Distancia con Dijkstra: " << laberintoArch.caminoMasCorto(idInicial, idFinal, camino) << "\n" << endl;

                camino.clear();

                int p = laberintoArch.caminoEncontrado(idInicial, idFinal, camino);
                cout << "Distancia con SACO: " << p << "\n" << endl;

                if (!p) {
                    cout << "\n el programa ha fallado en caminoEncontrado, todas las adyacencias poseen 0 de ferormona. Esto se debe a un error desconocido ajeno a la logica del codigo" << endl;
                    cout << "El fallo no ocurre siempre por lo que se puede ejecutar nuevamente el programa." << endl;
                    cout << "Si se depura el programa, y se entra manualmente en Simulador -> ejecutar() -> for() encargado de controlar los tics de reloj : " << endl;
                    cout << "Se puede utilizar el arreglo (double ady[18], adaptado para laberintop.txt) y ejecutar algunas iteraciones de forma manual para comprobar que las adyacencias se rellenan correctamente" << endl;
                }

            } catch (Error_Simulador e) {
                cout << e.what() << endl;
            }
        }

        continua = false;
    }
    return 0;
}
