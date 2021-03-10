/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Laberinto.cpp
 * Author: Alan
 * 
 * Created on 7 de febrero de 2018, 04:46 PM
 */

#include "Laberinto.h"

Laberinto::Laberinto(int cantidadVrts, double probabilidadAdy) : laberintoPtr(shared_ptr< Grafo< int, Adyacencia > > (new Grafo<int, Adyacencia>(cantidadVrts, probabilidadAdy))), laberinto(*laberintoPtr) {
}

Laberinto::Laberinto(ifstream& archivo) : laberintoPtr(shared_ptr< Grafo< int, Adyacencia > > (new Grafo<int, Adyacencia>(archivo))), laberinto(*laberintoPtr) {
}

Laberinto::Laberinto(const Laberinto& orig) : laberintoPtr(orig.laberintoPtr), laberinto(*laberintoPtr) {
}

Laberinto::~Laberinto() {
}

/* MÉTODOS OBSERVADORES BÁSICOS */
bool Laberinto::xstVrt(int idVrt) const {
    return laberinto.xstVrt(idVrt);
}

bool Laberinto::xstAdy(int idVrtO, int idVrtD) const {
    return laberinto.xstAdy(idVrtO, idVrtD);
}

int Laberinto::obtIdVrtInicial() const {
    return idVrtInicial;
}

int Laberinto::obtIdVrtFinal() const {
    return idVrtFinal;
}

void Laberinto::obtIdVrtAdys(int idVrt, vector< int >& rsp) const throw (Error_laberinto) {
    try {
        laberinto.obtIdVrtAdys(idVrt, rsp);
    } catch (Error_grafo e) {
        string rsl = e.what();
        throw Error_laberinto(rsl);
    }
}

Adyacencia Laberinto::obtDatoAdy(int idVrtO, int idVrtD) const throw (Error_laberinto) {
    try {
        return laberinto.obtDatoAdy(idVrtO, idVrtD);
    } catch (Error_grafo e) {
        string rsl = e.what();
        throw Error_laberinto(rsl);
    }
}

int Laberinto::obtTotAdy() const {
    return laberinto.obtTotAdy();
}

int Laberinto::obtTotVrt() const {
    return laberinto.obtTotVrt();
}

int Laberinto::caminoMasCorto(int idVrtO, int idVrtD, vector< int >& camino) const throw (Error_laberinto) {
    try {
        return laberinto.caminoMasCorto(idVrtO, idVrtD, camino);
    } catch (Error_grafo e) {
        string rsl = e.what();
        throw Error_laberinto(rsl);
    }
}

int Laberinto::caminoEncontrado(int idVrtO, int idVrtD, vector<int>& camino) const throw (Error_laberinto) {
    try {
        camino.push_back(idVrtO);
        encontrarCamino(idVrtD, camino); //Metodo Recursivo

        return camino.size() - 1;
    } catch (Error_grafo e) {
        string rsl = e.what();
        throw Error_laberinto(rsl);
    }
}

void Laberinto::encontrarCamino(int idVrtD, vector<int>& camino) const {
    int idTemp = -1;
    double mayor = 0.0;
    double ferormona;
    bool continua = true;
    vector<int> vecinos;
    laberinto.obtIdVrtAdys(camino.back(), vecinos); //Rellena el vector vecinos con los vecinos del vertice actual
    Adyacencia ad;
    try {
        if (camino.back() != idVrtD) {
            while (continua) {
                for (int i = 0; i < vecinos.size(); ++i) {
                    ad = laberinto.obtDatoAdy(camino.back(), vecinos[i]);
                    ferormona = ad.obtValoracion();
                    vector<int>::iterator iter = std::find(camino.begin(), camino.end(), vecinos[i]);
                    if (mayor < ferormona && iter == camino.end()) {
                        idTemp = i;
                        mayor = ferormona;
                    }
                }

                if (idTemp != -1) {
                    camino.push_back(vecinos[idTemp]);

                    vecinos[idTemp] = -1;
                    encontrarCamino(idVrtD, camino);

                    if (camino.back() != idVrtD) {
                        mayor = 0.0;
                        idTemp = -1;
                        camino.pop_back();
                    } else {
                        continua = false;
                    }

                } else {
                    continua = false;
                }
            }
        }
    } catch (Error_grafo e) {

    }
}

double Laberinto::sumaTotalFerormona() const throw (Error_laberinto) {
    vector<int> rsp;
    double suma = 0.0;
    try {
        for (int i = 0; i < laberinto.obtTotVrt(); i++) {
            laberinto.obtIdVrtAdys(i, rsp);
            for (int j = 0; j < rsp.size(); j++) {
                if (rsp[j] > i) {
                    suma += laberinto.obtDatoAdy(i, rsp[j]).obtCntFerormona();
                }
            }
        }

        return suma;
    } catch (Error_grafo e) {
        string rsl = e.what();
        throw Error_laberinto(rsl);
    }
}

void Laberinto::asgIdVrtInicial(int idVrtInicialN) {
    if (laberinto.xstVrt(idVrtInicialN)) {
        idVrtInicial = idVrtInicialN;
    }
}

void Laberinto::asgIdVrtFinal(int idVrtFinalN) {
    if (laberinto.xstVrt(idVrtFinalN)) {
        idVrtFinal = idVrtFinalN;
    }
}

void Laberinto::asgDatoAdy(int idVrtO, int idVrtD, const Adyacencia& ady) throw (Error_laberinto) {
    try {
        laberinto.asgDatoAdy(idVrtO, idVrtD, ady);
    } catch (Error_grafo e) {
        string rsl = e.what();
        throw Error_laberinto(rsl);
    }
}

void Laberinto::decrementarFerormonaAdys(double decrFerormona) throw (Error_laberinto) {
    vector<int> rsp;
    Adyacencia ad;
    try {
        for (int i = 0; i < laberinto.obtTotVrt(); i++) {
            laberinto.obtIdVrtAdys(i, rsp);
            for (int j = 0; j < rsp.size(); j++) {
                if (rsp[j] > i) {
                    ad = laberinto.obtDatoAdy(i, rsp[j]);
                    ad.asgCntFerormona(ad.obtCntFerormona() * decrFerormona);
                    laberinto.asgDatoAdy(i, rsp[j], ad);
                }
            }
        }
    } catch (Error_grafo e) {
        string rsl = e.what();
        throw Error_laberinto(rsl);
    }
}

void Laberinto::actualizarValoracionAdys() throw (Error_laberinto) {
    //cada valor de feromona entre el total de feromona
    double sumaTotal = sumaTotalFerormona();
    vector<int> rsp;
    Adyacencia ad;
    try {
        for (int i = 0; i < laberinto.obtTotVrt(); i++) {
            laberinto.obtIdVrtAdys(i, rsp);
            for (int j = 0; j < rsp.size(); j++) {
                if (rsp[j] > i) {
                    ad = laberinto.obtDatoAdy(i, rsp[j]);
                    ad.asgValoracion(ad.obtCntFerormona() / sumaTotal);
                    laberinto.asgDatoAdy(i, rsp[j], ad);
                }
            }
        }
    } catch (Error_grafo e) {
        string rsl = e.what();
        throw Error_laberinto(rsl);
    }
}