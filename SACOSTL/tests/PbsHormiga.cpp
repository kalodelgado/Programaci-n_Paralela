/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   newsimpletest.cpp
 * Author: John
 *
 * Created on 14 de mayo de 2018, 03:30 PM
 */

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "Hormiga.h"
#include <string>

/*
 * Simple C++ Test Suite
 */

void testMover() {


    ifstream archivo("labretroceso.txt", ios::in);
    if (!archivo) {
        std::cout << "%TEST_FAILED% time=0 testname=testMover (newsimpletest) No encuentra archivo" << std::endl;
    }
    Laberinto lbrt(archivo);



    //PRUEBA #1  LLEGA AL DESTINO, SE DEVUELVE Y SE DUERME

    lbrt.asgIdVrtInicial(0);
    lbrt.asgIdVrtFinal(6);
    Hormiga hormigaRetroceso;
    hormigaRetroceso.asgLaberinto(lbrt);

    cout << "Prueba Retroceso y se Duerme" << endl;
    for (int i = 0; i < 17; ++i) {
        hormigaRetroceso.mover();
        cout << hormigaRetroceso.obtMemoria() << endl;

        if (i == 1 && hormigaRetroceso.obtDestino() != 'F') {
            std::cout << "%TEST_FAILED% time=0 testname=testMover (newsimpletest) La hormiga no sigue la direccion esperada (F)" << std::endl;
        }

        if (i == 6 && hormigaRetroceso.obtMemoria() != "[0,1,2,3,4,5,6]") {
            std::cout << "%TEST_FAILED% time=0 testname=testMover (newsimpletest) La hormiga no siguio su camino correctamente hasta el destino" << std::endl;
        }

        if (i == 8 && hormigaRetroceso.obtDestino() != 'I') {
            std::cout << "%TEST_FAILED% time=0 testname=testMover (newsimpletest) La hormiga no sigue la direccion esperada (I)" << std::endl;
        }

        if (i == 14 && hormigaRetroceso.obtMemoria() != "[]") { // si = [] esta dormida
            std::cout << "%TEST_FAILED% time=0 testname=testMover (newsimpletest) La hormiga no siguio la memoria correctamente para retornar al vertice inicial" << std::endl;
        }
    }

    
    /*PRUEBA #2 LLEGA A UN CICLO Y SE DEVUELVE K PASOS*/
    ifstream archivo2("labciclo.txt", ios::in);
    Laberinto lbrt2(archivo2);
    string memoria = "";

    Adyacencia ady(0.0, 0.1);

    lbrt2.asgDatoAdy(1, 3, ady);
    lbrt2.asgDatoAdy(2, 3, ady);

    lbrt2.asgIdVrtInicial(0);
    lbrt2.asgIdVrtFinal(4);

    Hormiga hormigaCiclo;
    hormigaCiclo.asgLaberinto(lbrt2);
    
    cout << "Prueba Ciclo" << endl;
    for (int i = 0; i < 11; ++i) {
        hormigaCiclo.mover();
        cout << hormigaCiclo.obtMemoria() << endl;

        if (i == 3 && hormigaCiclo.obtMemoria() == "[0,1,3,2]") {
            memoria = hormigaCiclo.obtMemoria();
        }

        if (i == 4 && memoria != "" && hormigaCiclo.obtMemoria() != "[0,1,3]") {
            std::cout << "%TEST_FAILED% time=0 testname=testMover (newsimpletest) La hormiga no se devuelve a la posicion anterior" << std::endl;
        }

        if (i == 5 && memoria != "" && hormigaCiclo.obtMemoria() != "[0,1]") {
            std::cout << "%TEST_FAILED% time=0 testname=testMover (newsimpletest) La hormiga no se devuelve mas de una vez" << std::endl;
        }
    }

    /*PRUEBA #3 ENCUENTRA UN CALLEJON SIN SALIDA Y SE DEVUELVE*/
    ifstream archivo3("labsinsalida.txt", ios::in);
    Laberinto lbrt3(archivo3);
    memoria = "";

    Adyacencia ady2(0.0, 0.1);

    lbrt3.asgDatoAdy(0, 4, ady2);

    lbrt3.asgIdVrtInicial(0);
    lbrt3.asgIdVrtFinal(3);
    Hormiga hormigaSalida;
    hormigaSalida.asgLaberinto(lbrt3);
    
    cout << "Prueba camino sin salida" << endl;
    for (int i = 0; i < 11; ++i) {
        hormigaSalida.mover();
        cout << hormigaSalida.obtMemoria() << endl;

        if (i == 3 && hormigaSalida.obtMemoria() == "[0,4,6,5]") {
            memoria = hormigaSalida.obtMemoria();
        }

        if (i == 4 && memoria != "" && hormigaSalida.obtMemoria() != "[0,4,6]") {
            std::cout << "%TEST_FAILED% time=0 testname=testMover (newsimpletest) La hormiga no se devuelve a la posicion anterior" << std::endl;
        }

        if (i == 5 && memoria != "" && hormigaSalida.obtMemoria() != "[0,4]") {
            std::cout << "%TEST_FAILED% time=0 testname=testMover (newsimpletest) La hormiga no se devuelve mas de una vez" << std::endl;
        }
    }
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% newsimpletest" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% testMover (newsimpletest)" << std::endl;
    testMover();
    std::cout << "%TEST_FINISHED% time=0 testMover (newsimpletest)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

