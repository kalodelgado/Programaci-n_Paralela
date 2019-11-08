#!/usr/bin/python3
import random
from grafo import Grafo

def main():
    # genera semilla para numeros aleatorios, OJO: no se puede invocar en el constructor porque repite!!
    random.seed()           
    mi_grafo = Grafo()
    mi_grafo.init__random(10,0.5)
    for i in range(mi_grafo.obt_total_vrts()):
        print(mi_grafo[i])
        print(mi_grafo.obt_idvrt_adys(i))
    print("Total de vertices: ", mi_grafo.obt_total_vrts())
    print("Total de adyacencias: ", mi_grafo.obt_total_adys())
    
    # a continuacion se lee un grafo de un archivo json y se construye con el metodo de clase correspondiente:
    nom_arch_json_grafo = input("Digite el nombre del archivo json con el grafo: ")
    with open(nom_arch_json_grafo, 'r') as arch_json_grafo:
        mi_grafo = Grafo()
        mi_grafo.init__json(arch_json_grafo)
    
    for i in range(mi_grafo.obt_total_vrts()):
        print(mi_grafo[i])
        print(mi_grafo.obt_idvrt_adys(i))
    return

main()