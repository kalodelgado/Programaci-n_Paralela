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

#include "Error_laberinto.h"

Error_laberinto::Error_laberinto(string elemento) : elementoDelError(elemento) {
}

Error_laberinto::~Error_laberinto() {
}

const char* Error_laberinto::what() const noexcept {
    string rsl = "ERROR EN EL LABERINTO: ";
    rsl += elementoDelError;
    return rsl.c_str();
}
