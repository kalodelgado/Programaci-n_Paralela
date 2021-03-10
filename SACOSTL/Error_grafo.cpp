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

#include "Error_grafo.h"

Error_grafo::Error_grafo(int t_e, string elemento):t_error(t_e),elementoDelError(elemento) {
}

Error_grafo::~Error_grafo() {
}

const char* Error_grafo::what() const noexcept {
    switch (t_error) {
        case 1:{
            string rsl1("ERROR: identificador de vértice no existe");
            rsl1 = rsl1 + " " + elementoDelError;
            return rsl1.c_str();
            break;
        }
        case 2:{
            string rsl2("ERROR: adyacencia no existe");
            rsl2 = rsl2 + " " + elementoDelError;
            return rsl2.c_str();
            break;
        }    
        case 3:{
            string rsl3("ERROR: no existe camino entre vértices");
            rsl3 = rsl3 + " " + elementoDelError;
            return rsl3.c_str();
            break;
        }
    }
}
