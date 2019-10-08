#include <vector>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <iostream>
#include <string>
using namespace std;

class LectorArchivo
{
    public:
        LectorArchivo(ifstream& archivo, vector< vector<double> >& vec);

        // Destructor
        ~LectorArchivo();
        
        void escribirSalida(vector< vector<double> >& vecDatos, vector< vector<double> >& clusters, vector<int>& clusterAsigment);
};

LectorArchivo::LectorArchivo(ifstream& archivo, vector< vector<double> >& vec)
{
    char datoActual; //lee un numero del archivo
    char finLinea = ' ';
    int contVertice = 0; //nos dira el indice del vertice donde se debe agregar datoActual

    string num = "";
    while(finLinea != 10 && finLinea != ',')
    {
        archivo >> datoActual;
        num += datoActual;
        finLinea = archivo.get();
        if(finLinea != 10 && finLinea != ',')
            num += finLinea;        
    }

    while (!archivo.eof()) {
        while (!archivo.eof() && (finLinea != 10)) 
        {
            vec[contVertice].push_back( atof(num.c_str()) ); //se agrega el elemento a la lista

            num = "";
            archivo >> datoActual;
            num += datoActual;
            finLinea = archivo.get();
            while(!archivo.eof() && finLinea != 10 && finLinea != ',')
            {                
                num += finLinea;
                finLinea = archivo.get();
            }
        }
        vec[contVertice].push_back( atof(num.c_str()) ); //inserta el valor leido por el while superior
        if (!archivo.eof()) { //si sale del while por finLinea
            num = "";
            archivo >> datoActual;
            num += datoActual;
            finLinea = archivo.get();
            while(finLinea != 10 && finLinea != ',')
            {                
                num += finLinea;
                finLinea = archivo.get();
            }
            ++contVertice; //se aumenta la posicion el vertice 
        }    
    }
}

LectorArchivo::~LectorArchivo(){}

void LectorArchivo::escribirSalida(vector< vector<double> >& vecDatos, vector< vector<double> >& clusters, vector<int>& clusterAsigment)
{
    ofstream archivo;
    
    archivo.open("archivo salida.csv", ios::out);
    
    if( archivo.fail() )
    {
        cout << "Error en salida de archivo." << endl;
        exit(1);
    }
    
    for(int i = 0; i < clusters.size(); i++)
    {
        for(int k = 0; k < clusters[0].size(); k++)
        {
            archivo << clusters[i][k];
            if(k != clusters[0].size() -1)
                archivo << ", ";
            else
                archivo << "\n";
        }
        
        for(int j = 0; j < clusterAsigment.size(); j++)
            if(clusterAsigment[j] == i)
            {
                for(int k = 0; k < vecDatos[0].size(); k++)
                {
                    archivo << vecDatos[j][k];
                    if(k != vecDatos[0].size() -1)
                        archivo << ", ";
                    else
                        archivo << "\n";
                }
            }        
        archivo << "\n";
    }
}