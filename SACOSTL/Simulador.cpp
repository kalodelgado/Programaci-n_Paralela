/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Simulador.cpp
 * Author: Alan
 * 
 * Created on 19 de febrero de 2018, 02:53 PM
 */

#include "Simulador.h"

Simulador::Simulador(Laberinto& lbr) : laberinto(lbr) {
}

Simulador::Simulador(const Simulador& orig) : laberinto(orig.laberinto) {
}

Simulador::~Simulador() {
}

void Simulador::iniciar(int idVrtInicial, int idVrtFinal, int cntHrm, double decrFerormona) {
    // ANTES de crear las hormigas, asigna la referencia al laberinto que accesarán todas
    Hormiga::asgLaberinto(laberinto); // se asigna laberinto a hormigas
    if (laberinto.xstVrt(idVrtInicial) && laberinto.xstVrt(idVrtFinal)) {
        laberinto.asgIdVrtFinal(idVrtFinal);
        laberinto.asgIdVrtInicial(idVrtInicial);
        this->decrFerormona = decrFerormona;
        cantidadHormigas = cntHrm;
        hormigas.resize(cntHrm);

        for (int i = 0; i < cntHrm; ++i) {
            hormigas[i].asgLaberinto(laberinto);
        }

    }
}

void Simulador::ejecutar(int p) throw (Error_Simulador) {

    try {
        int cntActual = 0;

        for (int i = 0; i < p; i++) {
            if (cntActual < cantidadHormigas) {
                cntActual += ((rand() % 6) + 5);
                if (cntActual > cantidadHormigas) {
                    cntActual = cantidadHormigas;
                }
            }
            for (int j = 0; j < cntActual; j++) {
                hormigas[j].mover();
            }
            laberinto.decrementarFerormonaAdys(decrFerormona);
            laberinto.actualizarValoracionAdys();

        }
    } catch (Error_laberinto e) {
        string rsl = e.what();
        throw Error_Simulador(rsl);
    }
}