/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Hormiga.cpp
 * Author: Alan
 * 
 * Created on 7 de febrero de 2018, 05:53 PM
 */

#include "Hormiga.h"

Laberinto* Hormiga::laberinto_p = 0;    

Hormiga::Hormiga() {
    haSalido = false;
    haRegresado = false;
    enRetroceso = 0;
}

Hormiga::Hormiga(const Hormiga& orig) {
    idVrtActual = orig.idVrtActual;

    memoria.resize(orig.laberinto_p->obtTotVrt()); //se asigna el tamaño al vector de memoria

    for (int i = 0; i < orig.laberinto_p->obtTotVrt(); ++i) {
        memoria[i] = orig.memoria[i];
    }

    haSalido = orig.haSalido;
    haRegresado = orig.haRegresado;
    destino = orig.destino;
    enRetroceso = orig.enRetroceso;
    longitudSolucion = orig.longitudSolucion;
    deltaFerormona = orig.deltaFerormona;

    laberinto_p = orig.laberinto_p;
}

Hormiga::~Hormiga() {
}

/* OBSERVADORES */

bool Hormiga::salio() {
    return haSalido;
}

bool Hormiga::regreso() {
    return haRegresado;
}

char Hormiga::obtDestino() {
    return destino;
}

string Hormiga::obtMemoria() {
    stringstream fs; //construye una instancia de flujo de salida
    std::vector< int >::iterator ite;

    fs << "[";

    if (!haRegresado) {
        for (ite = memoria.begin(); ite != memoria.end(); ++ite) {
            fs << *ite;
            if (ite + 1 != memoria.end())
                fs << ',';
        }
    }
    fs << "]";
    return fs.str();
}

/* METODOS MODIFICADORES */

void Hormiga::salir() {
    haSalido = true;
    idVrtActual = laberinto_p->obtIdVrtInicial();
    destino = 'F';
}

void Hormiga::mover() throw (Error_laberinto) {

    try {
        int seleccion;

        if (!memoria.size()) {//Si la hormiga aun no tiene memoria asignada se le asigna, despues se indica que la hormiga esta activa (haSalido)
            salir(); //se utiliza el metodo salir aunque podria hacerse su logica aqui, se utiliza por ser un metodo brindado por el prof
        }

        if (idVrtActual != laberinto_p->obtIdVrtFinal()) { //Pregunta si el vertice actual donde esta la hormiga es el vertice final
            if (!haRegresado) { //Pregunta si la hormiga no ha regresado 
                if (destino == 'F') { //Pregunta si la hormiga aun se dirige en busca de la posicion final 
                    if (enRetroceso > 0) { //Pregunta si la hormiga esta en retroceso por haber encontrado un camino sin salida o un ciclo 
                        if (memoria.back() != idVrtActual) {
                            idVrtActual = memoria.back();
                            memoria.pop_back();
                            --enRetroceso;
                        } else {
                            memoria.pop_back();
                            --enRetroceso;
                        }
                    } else {

                        seleccion = seleccionaAdyMasCargada();

                        if (seleccion == -1) {
                            srand(time(NULL));
                            memoria.push_back(idVrtActual); // Guarda el valor en memoria
                            enRetroceso = (rand() % (memoria.size() + 1)); // Un numero aleatorio entre 2 y total de memoria;
                            if (enRetroceso <= 1) {
                                enRetroceso += 2;
                            }

                        } else {
                            memoria.push_back(idVrtActual); // Guarda el valor en memoria
                            idVrtActual = seleccion; //salta al siguiente, lo declara idVertAct
                        }
                    }

                } else {
                    //ACA LA HORMIGA SE REGRESA UNA POCISION EN CADA CLOCK ASIGNANDO FERORMONA A LA ADYACENCIA POR LA QUE PASA

                    Adyacencia adyTemp = laberinto_p->obtDatoAdy(idVrtActual, memoria.back());
                    double asignacion = adyTemp.obtCntFerormona();
                    asignacion += deltaFerormona;
                    adyTemp.asgCntFerormona(asignacion);
                    laberinto_p->asgDatoAdy(idVrtActual, memoria.back(), adyTemp);

                    idVrtActual = memoria.back();
                    memoria.pop_back();
                    if (!memoria.size()) { //La hormiga ha llegado a su posicion inicial, entonces se duerme
                        haRegresado = true;
                    }
                }
            }
        } else { //Un paso que realiza cuando se encuentra sobre la ultima posicion, la hormiga se gira y cambia de direccion

            if (destino == 'F') {
                destino = 'I';
                memoria.push_back(idVrtActual);
                enRetroceso = -1;
                //ACA SE CALCULA DELTA FERORMONA
                deltaFerormona = 1.0 / (double) (memoria.size());
            } else {
                if (memoria.back() == idVrtActual) {
                    memoria.pop_back();
                } else {
                    Adyacencia adyTemp = laberinto_p->obtDatoAdy(idVrtActual, memoria.back());
                    double asignacion = adyTemp.obtCntFerormona();
                    asignacion += deltaFerormona;
                    adyTemp.asgCntFerormona(asignacion);
                    laberinto_p->asgDatoAdy(idVrtActual, memoria.back(), adyTemp);

                    idVrtActual = memoria.back();
                    memoria.pop_back();
                    if (!memoria.size()) { //La hormiga ha llegado a su posicion inicial, entonces se duerme
                        haRegresado = true;
                    }
                }
            }
        }
    } catch (Error_grafo e) {
        string rsl = e.what();
        throw Error_laberinto(rsl);
    }
}

void Hormiga::asgLaberinto(Laberinto& lbrt) {
    laberinto_p = &lbrt;
}

/* METODOS PRIVADOS SUGERIDOS */

void Hormiga::filtraVrtsPosibles(vector<int>& vrtsPosibles) { //De todos los vecinos posibles, eliminar los que ya estan en memoria

    for (int i = 0; i < vrtsPosibles.size(); ++i) {
        if (estaEnMemoria(vrtsPosibles[i])) {
            vrtsPosibles[i] = -1;
        }
    }


    std::vector<int>::iterator iter = std::remove_if(vrtsPosibles.begin(), vrtsPosibles.end(), [](int i) {
        return i == -1;;
    });
    vrtsPosibles.erase(iter, vrtsPosibles.end());

}

bool Hormiga::estaEnMemoria(int idVrt) { //Pregunta si un vertice ya se encuentra en la memoria
    bool esta = false;
    for (int v = 0; v < memoria.size() && !esta; ++v) {
        if (idVrt == memoria[v]) {
            esta = true;
        }
    }
    return esta;
}

int Hormiga::seleccionaAdyMasCargada() throw (Error_laberinto) { //Retorna la seleccion aleatoria de la hormiga, o un -1 si no se puede mover

    srand(time(NULL));
    int resultado; // retorna el id del vertice vecino cuya adyacencia fue seleccionada aleatoriamente por la hormiga
    double random = 0; //numero aleatorio que indica que posicion del arreglo de seleccion se va a utilizar
    std::vector<int> vecinos; //arreglo que almacena los vertices vecinos del vertice actual
    bool tieneCero = false; //Asume que no existen adyacencias con un valor mayor a cero
    bool tieneNumero = false; //Asume que no existen adyacencias con un valor mayor a cero
    int seleccion; //almacena la posicion seleccionada por rand()
    double ferormonaAdy; //valor temporal que guarda la ferormona de una adyacencia
    double sumatoriaVecinos = 0.0; // Es la suma total que se utiliza para normalizar los vecinos 
    vector<double> rangos; //almacena las valorizaciones de las adyacencias de forma proporcional
    vector<int> vecinosCero; // En caso de que existan ceros y numeros, almacena los vecinos con valor cero
    double valor; //valor temporal de valoracion
    int indice;
    double bandera;

    try {

        laberinto_p->obtIdVrtAdys(idVrtActual, vecinos); //guarda en un arreglo int los vertices de los vecinos del nodo actual

        filtraVrtsPosibles(vecinos); //Filtra todos los vecinos que ya esten en memoria y los elimina del vector

        /**********************SELECCION DE VECINOS*************************/

        if (vecinos.size() > 0) { //Pregunta si hay al menos un vecino que no este ya en memoria
            for (int i = 0; i < vecinos.size(); ++i) { // Se observan todos los posibles vecinos del vertice actual, para ver si alguno tiene un valor de cero de ferormona
                ferormonaAdy = laberinto_p->obtDatoAdy(idVrtActual, vecinos[i]).obtValoracion();
                if (ferormonaAdy > 0.0) { //encontró un valor mayor a cero
                    tieneNumero = true;
                } else {
                    tieneCero = true;
                }
            }

            if (tieneCero && !tieneNumero) { //VECINOS SOLO TIENEN CERO Y NO TIENEN NUMEROS
                seleccion = rand() % vecinos.size();
                resultado = vecinos[seleccion];
            } else {
                if (!tieneCero && tieneNumero) { //VECINOS NO TIENEN CERO Y SOLO TIENEN NUMEROS

                    bandera = 1.0; // No tiene ceros, se usa esta bandera
                    rangos.resize(vecinos.size());

                } else { //VECINOS TIENEN NUMEROS Y TAMBIEN TIENEN CEROS

                    bandera = 0.1; //Tiene ceros, se usa esta bandera

                    for (int i = 0; i < vecinos.size(); ++i) {
                        if (laberinto_p->obtDatoAdy(idVrtActual, vecinos[i]).obtValoracion() == 0.0) {
                            vecinosCero.push_back(vecinos[i]); //agrega el elemento al vector de ceros
                            vecinos[i] = -1; //coloca una bandera para eliminar al vecino
                        }
                    }
                    std::vector<int>::iterator iter = std::remove_if(vecinos.begin(), vecinos.end(), [](int i) {
                        return i == -1;;
                    });
                    vecinos.erase(iter, vecinos.end());

                    rangos.resize(vecinos.size());
                }

                /***********************FIN SELECCION DE VECINOS**************************/


                for (int i = 0; i < vecinos.size(); ++i) {
                    sumatoriaVecinos += laberinto_p->obtDatoAdy(idVrtActual, vecinos[i]).obtValoracion();
                }

                for (int i = 0; i < vecinos.size(); ++i) {
                    valor = (laberinto_p->obtDatoAdy(idVrtActual, vecinos[i]).obtValoracion() * bandera) / (sumatoriaVecinos * bandera);
                    rangos[i] = valor;
                }

                /*ORDENA EL VECTOR DE VALORIZACIONES*/
                int minimo;
                double temporalrango;
                int temporalvecino;


                for (int i = 0; i < rangos.size() - 1; ++i) {
                    minimo = i;
                    for (int j = i + 1; j < rangos.size(); ++j) {
                        if (rangos[j] < rangos[minimo]) {
                            minimo = j;
                        }
                    }

                    temporalvecino = vecinos[minimo];
                    vecinos[minimo] = vecinos[i];
                    vecinos[i] = temporalvecino;

                    temporalrango = rangos[minimo];
                    rangos[minimo] = rangos[i];
                    rangos[i] = temporalrango;
                }

                while (!random) {
                    random = (double) (rand() % 1000) / 1000.0; //acá selecciona el valor random
                }

                if (random < 0.1 && tieneCero) {
                    double random2 = rand() % vecinosCero.size();
                    resultado = vecinosCero[random2];
                } else {
                    if (tieneCero) {
                        bandera = 0.1;
                    } else {
                        bandera = 0.0;
                    }
                    indice = -1;
                    while (random > bandera) {
                        ++indice;
                        bandera += rangos[indice];
                    }
                    resultado = vecinos[indice];
                }

            }
        } else {
            resultado = -1;
        }

        return resultado;

    } catch (Error_grafo e) {
        string rsl = e.what();
        throw Error_laberinto(rsl);
    }
}

bool Hormiga::laberintoAsignado() {
    return laberinto_p != 0;
}
