/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Error_plantilla.h
 * Author: John
 *
 * Created on 18 de junio de 2018, 10:49 AM
 */

#ifndef ERROR_SIMULADOR_H
#define ERROR_SIMULADOR_H

#include <exception>
#include <string>
using namespace std;

class Error_Simulador : public exception {
public:
    Error_Simulador(string elemento);
    virtual ~Error_Simulador();

    // retorna una hilera que describe el error
    virtual const char* what() const noexcept;

private:
    string elementoDelError;
};

#endif /* ERROR_SIMULADOR_H */

