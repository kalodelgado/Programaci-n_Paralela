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

#ifndef ERROR_GRAFO_H
#define ERROR_GRAFO_H

#include <exception>
#include <string>
using namespace std;

class Error_grafo : public exception {
public:
    Error_grafo(int t_e, string elemento);
    virtual ~Error_grafo();

    // retorna una hilera que describe el error
    virtual const char* what() const noexcept;

private:
    int t_error;
    string elementoDelError;
};

#endif /* ERROR_GRAFO_H */

