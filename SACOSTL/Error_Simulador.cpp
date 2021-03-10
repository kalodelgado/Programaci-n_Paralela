/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Error_plantilla.cpp
 * Author: John
 * 
 * Created on 18 de junio de 2018, 10:49 AM
 */

#include "Error_Simulador.h"

Error_Simulador::Error_Simulador(string elemento) : elementoDelError(elemento) {
}

Error_Simulador::~Error_Simulador() {
}

const char* Error_Simulador::what() const noexcept {
    string rsl = "ERROR EN EL SIMULADOR: ";
    rsl += elementoDelError;
    return rsl.c_str();
}
