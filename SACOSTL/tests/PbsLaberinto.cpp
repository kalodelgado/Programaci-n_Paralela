/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   newsimpletest.cpp
 * Author: John
 *
 * Created on 16 de abril de 2018, 08:00 AM
 */

#include <stdlib.h>
#include <iostream>
#include "Laberinto.h"
#include "Hormiga.h"

/*
 * Simple C++ Test Suite
 */

void testLaberinto() {
    int cantidadVrts;
    int totalVertices;
    double probabilidadAdy;
    double totalAdyacencias;
    double promedio;
    totalAdyacencias = 0;
    cantidadVrts = 50;
    probabilidadAdy = 0.3;
    promedio = probabilidadAdy * (cantidadVrts * (cantidadVrts - 1));

    for (int i = 0; i < 100; ++i) {
        Laberinto laberinto(cantidadVrts, probabilidadAdy);
        totalAdyacencias += laberinto.obtTotAdy();

        if (i == 0) {
            totalVertices = laberinto.obtTotVrt();
        }
    }

    totalAdyacencias = totalAdyacencias / 100.0;

    if (cantidadVrts != totalVertices) {
        std::cout << "%TEST_FAILED% time=0 testname=testLaberinto (newsimpletest) Fallo constructor laberinto con cntVrt = 50, no coincide cantidad de vertices" << std::endl;
    }

    if (!(promedio - 50.0 <= totalAdyacencias && totalAdyacencias <= promedio + 50.0)) {
        std::cout << "%TEST_FAILED% time=0 testname=testLaberinto (newsimpletest) Fallo constructor laberinto con cntVrt = 50, en el promedio de adyacencias" << std::endl;
    }

    totalAdyacencias = 0;
    cantidadVrts = 10;
    probabilidadAdy = 0.3;
    promedio = probabilidadAdy * (cantidadVrts * (cantidadVrts - 1));

    for (int i = 0; i < 600; ++i) {
        Laberinto laberinto(cantidadVrts, probabilidadAdy);
        totalAdyacencias += laberinto.obtTotAdy();

        if (i == 0) {
            totalVertices = laberinto.obtTotVrt();
        }
    }

    totalAdyacencias = totalAdyacencias / 600.0;

    if (cantidadVrts != totalVertices) {
        std::cout << "%TEST_FAILED% time=0 testname=testLaberinto (newsimpletest) Fallo constructor laberinto con cntVrt = 10, no coincide cantidad de vertices" << std::endl;
    }

    if (!(promedio - 50.0 <= totalAdyacencias && totalAdyacencias <= promedio + 50.0)) {
        std::cout << "%TEST_FAILED% time=0 testname=testLaberinto (newsimpletest) Fallo constructor laberinto con cntVrt = 10, en el promedio de adyacencias" << std::endl;
    }
}

void testLaberinto2() {
    ifstream archivo("laberintop.txt", ios::in);
    ifstream archivolab1("laberintop.txt", ios::in);

    Laberinto laberinto(archivolab1);

    bool esValido = true;
    bool seguir = true;
    if (!archivo) { // operador ! sobrecargado
        esValido = false;
    }

    int fallo;
    int salto; //cada que cambia de reglon
    int contAdyacenciaVertice = 0;
    int contVertice = 0; //nos indica el indice del arregloVrt que evaluaremos su CntAdy con el contAdyacencia
    int totalVertices; //se utilizarÃ¡ en la primera lectura
    char finLinea = ' ';

    if (esValido) {

        archivo >> totalVertices;

        if (totalVertices != laberinto.obtTotVrt()) {
            std::cout << "%TEST_FAILED% time=0 testname=testLaberinto2 (newsimpletest) La cantidad de vertices no coincide en el laberintop" << std::endl;
        }

        archivo >> salto;
        while (!archivo.eof() && seguir) {
            while (!archivo.eof() && (finLinea != 10) && seguir) {
                ++contAdyacenciaVertice;
                archivo >> salto;
                archivo.get(); // consume un blanco
                finLinea = archivo.peek();
            }
            if (!archivo.eof()) {
                ++contAdyacenciaVertice;
            }
            archivo >> salto;
            archivo.get();
            finLinea = archivo.peek();
            if (laberinto.obtCntAdy(contVertice) != contAdyacenciaVertice) {
                seguir = false;
                fallo = contVertice;
            }
            contAdyacenciaVertice = 0; //se vuelve cero para evaluarlo con el siguiente vertice
            ++contVertice;
        }
    }


    if (!esValido) {
        std::cout << "%TEST_FAILED% time=0 testname=testLaberinto2 (newsimpletest) No abre el archivo pequeno o formato de archivo no valido" << std::endl;
    }

    //Si seguir es falso quiere decir que la cantidad de adyacencias no es el mismo
    if (!seguir) {
        std::cout << "%TEST_FAILED% time=0 testname=testLaberinto2 (newsimpletest) No coincide el numero de adyacencias para pruebas de archivo pequeño en el vertice #:" << fallo << std::endl;
    }

    ifstream archivo2("laberintom.txt", ios::in);
    ifstream archivolab2("laberintom.txt", ios::in);

    Laberinto laberinto2(archivolab2);

    esValido = true;
    seguir = true;

    if (!archivo2) { // operador ! sobrecargado
        esValido = false;
    }

    contAdyacenciaVertice = 0;
    contVertice = 0;

    if (esValido) {

        archivo2 >> totalVertices;

        if (totalVertices != laberinto2.obtTotVrt()) {
            std::cout << "%TEST_FAILED% time=0 testname=testLaberinto2 (newsimpletest) La cantidad de vertices no coincide en el laberintom" << std::endl;
        }

        archivo2 >> salto;
        while (!archivo2.eof() && seguir) {
            while (!archivo2.eof() && (finLinea != 10) && seguir) {
                ++contAdyacenciaVertice;
                archivo2 >> salto;
                archivo2.get(); // consume un blanco
                finLinea = archivo2.peek();
            }
            if (!archivo2.eof()) {
                ++contAdyacenciaVertice;
            }
            archivo2 >> salto;
            archivo2.get();
            finLinea = archivo2.peek();
            if (laberinto2.obtCntAdy(contVertice) != contAdyacenciaVertice) {
                seguir = false;
                fallo = contVertice;
            }
            contAdyacenciaVertice = 0;
            ++contVertice;
        }
    }

    if (!esValido) {
        std::cout << "%TEST_FAILED% time=0 testname=testLaberinto2 (newsimpletest) No abre el archivo mediano o formato de archivo no valido" << std::endl;
    }

    if (!seguir) {
        std::cout << "%TEST_FAILED% time=0 testname=testLaberinto2 (newsimpletest) No coincide el numero de adyacencias para pruebas de archivo mediano en el vertice #:" << fallo << std::endl;
    }
}

void testLaberinto3() {

    ifstream archivo("laberintop.txt", ios::in);
    //Laberinto orig(50, 0.3);  
    Laberinto laberintoArchivo(archivo);
    Laberinto laberinto(laberintoArchivo);
    int totalVertices = laberinto.obtTotVrt();
    int totalAdyacencias = laberinto.obtTotAdy();
    bool seguir = true;

    if (totalVertices != laberintoArchivo.obtTotVrt() || totalAdyacencias != laberintoArchivo.obtTotAdy()) {
        std::cout << "%TEST_FAILED% time=0 testname=testLaberinto3 (newsimpletest) Fallo constructor por copia del laberintop, total de vertices o total de adyacencias no coinciden" << std::endl;
    }

    for (int i = 0; i < totalVertices && seguir; ++i) {
        if (laberinto.obtCntAdy(i) != laberintoArchivo.obtCntAdy(i)) {
            seguir = false;
        }
    }

    //Entra en el if si no coincide la cantidad de adyacencia
    if (!seguir) {
        std::cout << "%TEST_FAILED% time=0 testname=testLaberinto3 (newsimpletest) Fallo constructor por copia en laberintop, evaluando adyacencias de cada vertice" << std::endl;
    }

    ifstream archivo2("laberintom.txt", ios::in);
    Laberinto laberintoArchivo2(archivo2);
    Laberinto laberinto2(laberintoArchivo2);
    totalVertices = laberinto2.obtTotVrt();
    totalAdyacencias = laberinto2.obtTotAdy();
    seguir = true;

    if (totalVertices != laberintoArchivo2.obtTotVrt() || totalAdyacencias != laberintoArchivo2.obtTotAdy()) {
        std::cout << "%TEST_FAILED% time=0 testname=testLaberinto3 (newsimpletest) Fallo constructor por copia en el laberintom, total de vertices o total de adyacencias no coinciden" << std::endl;
    }

    for (int i = 0; i < totalVertices && seguir; ++i) {
        if (laberinto2.obtCntAdy(i) != laberintoArchivo2.obtCntAdy(i)) {
            seguir = false;
        }
    }

    if (!seguir) {
        std::cout << "%TEST_FAILED% time=0 testname=testLaberinto3 (newsimpletest) Fallo constructor por copia en laberintom, evaluando adyacencias de cada vertice" << std::endl;
    }
}

void testSumaTotalFerormona() {
    ifstream archivo("laberintop.txt", ios::in);

    Laberinto laberinto(archivo);

    Adyacencia valAdy(1.3, 1.0);
    //= 1.3;
    double result;

    if (laberinto.xstAdy(0, 1)) { //verifica la existencia de la adyacencia y asigna
        laberinto.asgDatoAdy(0, 1, valAdy);
    }
    if (laberinto.xstAdy(1, 5)) {
        laberinto.asgDatoAdy(1, 5, valAdy);
    }
    if (laberinto.xstAdy(6, 8)) {
        laberinto.asgDatoAdy(6, 8, valAdy);
    }
    result = laberinto.sumaTotalFerormona();

    if (result != (valAdy.obtCntFerormona()*2)) {
        std::cout << "%TEST_FAILED% time=0 testname=testSumaTotalFerormona (newsimpletest) Falla con Total de Feromona asignando valores de adyacencia" << std::endl;
    }

    laberinto.decrementarFerormonaAdys(0.1);
    result = laberinto.sumaTotalFerormona();
    if (result != ((valAdy.obtCntFerormona() - 0.1)*2)) {
        std::cout << "%TEST_FAILED% time=0 testname=testSumaTotalFerormona (newsimpletest) Falla con Total de Feromona decrementando los valores de adyacencia" << std::endl;
    }
}

void testCaminoMasCorto() {
    int idVrtO;
    int idVrtD;

    ifstream archivo("laberintop.txt", ios::in);

    Laberinto laberinto(archivo);
    idVrtO = 4;
    idVrtD = 7;
    std::vector<int> camino;

    int result = laberinto.caminoMasCorto(idVrtO, idVrtD, camino);
    if (result != 2) {
        std::cout << "%TEST_FAILED% time=0 testname=testCaminoMasCorto (newsimpletest) No encontro el camino mas corto con dos de total" << std::endl;
    }

    if (camino[0] != 7 || camino[1] != 6 || camino[2] != 4) {
        std::cout << "%TEST_FAILED% time=0 testname=testCaminoMasCorto (newsimpletest) El vector camino no coincide con camino (7,6,4)" << std::endl;
    }

    idVrtO = 0;
    idVrtD = 1;
    camino.clear();
    result = laberinto.caminoMasCorto(idVrtO, idVrtD, camino);
    if (result != 1) {
        std::cout << "%TEST_FAILED% time=0 testname=testCaminoMasCorto (newsimpletest) No encontrÃƒÂ³ el camino mÃƒÂ¡s corto con uno de total" << std::endl;
    }

    if (camino[0] != 1 || camino[1] != 0) {
        std::cout << "%TEST_FAILED% time=0 testname=testCaminoMasCorto (newsimpletest) El vector camino no coincide con camino (1,0)" << std::endl;
    }

    idVrtO = 3;
    idVrtD = 7;
    camino.clear();
    result = laberinto.caminoMasCorto(idVrtO, idVrtD, camino);
    if (result != 3) {
        std::cout << "%TEST_FAILED% time=0 testname=testCaminoMasCorto (newsimpletest) No encontrÃƒÂ³ el camino mÃƒÂ¡s corto con tres de total" << std::endl;
    }

    if (camino[0] != 7 || camino[1] != 2 || camino[2] != 8 || camino[3] != 3) {
        std::cout << "%TEST_FAILED% time=0 testname=testCaminoMasCorto (newsimpletest) El vector camino no coincide con camino (7,6,...,3)" << std::endl;
    }
}

/*
void testCaminoEncontrado() {
    int idVrtO;
    int idVrtD;
    int result;

    ifstream archivo("laberintop.txt", ios::in);

    Laberinto lab(archivo);
    Laberinto lab2(archivo);
    Laberinto lab3(archivo);

    std::vector<int> camino;
    int caminoPRUEBA[10]; //PRUEBA


    idVrtO = 4;
    idVrtD = 7;
    lab.asgIdVrtInicial(idVrtO);
    lab.asgIdVrtFinal(idVrtD);

    Hormiga hormigas1[300];
    int cntActual = 0;

    for (int i = 0; i < 300; ++i) {
        hormigas1[i].asgLaberinto(lab);
    }

    for (int i = 0; i < 100; i++) {
        if (cntActual < 300) {
            cntActual += ((rand() % 20) + 20);
            if (cntActual > 300) {
                cntActual = 300;
            }
        }
        for (int j = 0; j < cntActual; ++j) {
            hormigas1[j].mover();
        }
    }

    result = lab.caminoEncontrado(4, 7, camino);

    for (int i = 0; i < 10; ++i) {//Rellena el vector de vecinos de PRUEBA
        if (i < camino.size()) {
            caminoPRUEBA[i] = camino[i];
        } else {
            caminoPRUEBA[i] = -1;
        }
    }


    if (result != 3) {
        std::cout << "%TEST_FAILED% time=0 testname=testCaminoMasCorto (newsimpletest) No encontro el camino segun la cantidad de feromona con result = 3" << std::endl;
    }

    idVrtO = 0;
    idVrtD = 1;
    lab2.asgIdVrtInicial(idVrtO);
    lab2.asgIdVrtFinal(idVrtD);
    camino.clear();

    Hormiga hormigas2[300];

    for (int i = 0; i < 300; ++i) {
        hormigas2[i].asgLaberinto(lab2);
    }

    for (int i = 0; i < 100; i++) {
        if (cntActual < 300) {
            cntActual += ((rand() % 20) + 20);
            if (cntActual > 300) {
                cntActual = 300;
            }
        }
        for (int j = 0; j < cntActual; ++j) {
            hormigas2[j].mover();
        }
    }

    result = lab2.caminoEncontrado(0, 1, camino);

    for (int i = 0; i < 10; ++i) {//Rellena el vector de vecinos de PRUEBA
        if (i < camino.size()) {
            caminoPRUEBA[i] = camino[i];
        } else {
            caminoPRUEBA[i] = -1;
        }
    }

    if (result != 2) {
        std::cout << "%TEST_FAILED% time=0 testname=testCaminoMasCorto (newsimpletest) No encontro el camino segun la cantidad de feromona con result = 2" << std::endl;
    }

    idVrtO = 3;
    idVrtD = 7;
    lab3.asgIdVrtInicial(idVrtO);
    lab3.asgIdVrtFinal(idVrtD);
    camino.clear();

    Hormiga hormigas3[300];

   for (int i = 0; i < 300; ++i) {
        hormigas3[i].asgLaberinto(lab3);
    }

    for (int i = 0; i < 100; i++) {
        if (cntActual < 300) {
            cntActual += ((rand() % 20) + 20);
            if (cntActual > 300) {
                cntActual = 300;
            } 
        }
        for (int j = 0; j < cntActual; ++j) {
            hormigas3[j].mover();
        }
    }

    result = lab3.caminoEncontrado(3, 7, camino);

    for (int i = 0; i < 10; ++i) {//Rellena el vector de vecinos de PRUEBA
        if (i < camino.size()) {
            caminoPRUEBA[i] = camino[i];
        } else {
            caminoPRUEBA[i] = -1;
        }
    }

    if (result != 4) {
        std::cout << "%TEST_FAILED% time=0 testname=testCaminoMasCorto (newsimpletest) No encontro el camino segun la cantidad de feromona con result = 4" << std::endl;
    }

}
 */

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% newsimpletest" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% testLaberinto (newsimpletest)" << std::endl;
    testLaberinto();
    std::cout << "%TEST_FINISHED% time=0 testLaberinto (newsimpletest)" << std::endl;

    std::cout << "%TEST_STARTED% testLaberinto2 (newsimpletest)" << std::endl;
    testLaberinto2();
    std::cout << "%TEST_FINISHED% time=0 testLaberinto2 (newsimpletest)" << std::endl;

    std::cout << "%TEST_STARTED% testLaberinto3 (newsimpletest)" << std::endl;
    testLaberinto3();
    std::cout << "%TEST_FINISHED% time=0 testLaberinto3 (newsimpletest)" << std::endl;

    std::cout << "%TEST_STARTED% testSumaTotalFerormona (newsimpletest)" << std::endl;
    testSumaTotalFerormona();
    std::cout << "%TEST_FINISHED% time=0 testSumaTotalFerormona (newsimpletest)" << std::endl;

    std::cout << "%TEST_STARTED% testCaminoMasCorto (newsimpletest)" << std::endl;
    testCaminoMasCorto();
    std::cout << "%TEST_FINISHED% time=0 testCaminoMasCorto (newsimpletest)" << std::endl;
    /*
        std::cout << "%TEST_STARTED% testCaminoEncontrado (newsimpletest)" << std::endl;
        testCaminoEncontrado();
        std::cout << "%TEST_FINISHED% time=0 testCaminoEncontrado (newsimpletest)" << std::endl;
     */
    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}





