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
        
        void escribirSalida(vector< vector<double> >& vecDatos, vector< vector<double> >& clusters, vector<int>& clusterAsigment, double tiempo, double costo);
};

LectorArchivo::LectorArchivo(ifstream& archivo, vector< vector<double> >& vec)
{
	char finLinea = ' ';
	int contVertice = 0; //nos dira el indice del vertice donde se debe agregar datoActual

	string num = "";
	while (finLinea != 10 && finLinea != ',')
	{
		finLinea = archivo.get();
		if (finLinea != 10 && finLinea != ',')
			num += finLinea;
	}

	while (!archivo.eof()) {
		while (!archivo.eof() && (finLinea != 10))
		{
			vec[contVertice].push_back(atof(num.c_str())); //se agrega el elemento a la lista

			num = "";
			finLinea = archivo.get();
			while (!archivo.eof() && finLinea != 10 && finLinea != ',')
			{
				num += finLinea;
				finLinea = archivo.get();
			}
		}
		vec[contVertice].push_back(atof(num.c_str())); //inserta el valor leido por el while superior
		if (!archivo.eof()) { //si sale del while por finLinea
			num = "";
			finLinea = archivo.get();
			while (finLinea != 10 && finLinea != ',' && !archivo.eof())
			{
				num += finLinea;
				finLinea = archivo.get();
			}
			++contVertice; //se aumenta la posicion el vertice 
		}
	}
}

LectorArchivo::~LectorArchivo(){}

void LectorArchivo::escribirSalida(vector< vector<double> >& vecDatos, vector< vector<double> >& clusters, vector<int>& clusterAsigment, double tiempo, double costo)
{
    ofstream archivo;
	int contador;
    
    archivo.open("salida.csv", ios::out);
    
    if( archivo.fail() )
    {
        cout << "Error en salida de archivo." << endl;
        exit(1);
    }
    
    for(int i = 0; i < (int)clusters.size(); i++)
    {
		contador = 0;
        for(int k = 0; k < (int)clusters[0].size(); k++)
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
				contador++;
            }        
        archivo << contador << "\n\n";
    }
	archivo << "Costo " << costo << "\n";
	archivo << "Duracion " << tiempo << " segundos.";

}