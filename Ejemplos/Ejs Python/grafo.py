#!/usr/bin/python3

# #!/usr/bin/python3 is a shebang line, without it the operating system does not know it's a python script, 
# even if you set the execution flag on the script and run it like ./script.py. 
# To make the script run by default in python3, either invoke it as python3 script.py or set the shebang line.

import random
import math
import copy
import json
import numpy as np

class Grafo:
    """Representa una red compleja de vertices (vrt) y adyacencias (ady).
       SUPUESTOS: 
       1. los vertices se identifican con enteros entre 0 y n"""
    
    class Vertice:
        def __init__(self):
            self.vrt = None  # vertice
            self.adys = []    # lista de adyacencias de vrt
        
        @classmethod
        def de_json(cls, vertice_json: str) -> 'Vertice':
            return 
       
    # Construye un grafo vacio.
    # Se requiere luego invocar alguno de los inicializadores adicionales.
    def __init__(self):
        self.vertices = []          # se crea el arreglo de vertices vacio

    # REQ: 0 < p < 1.
    # Construye un grafo con n vertices en el que el conjunto de
    # adyacencias se determina aleatoriamente utilizando p como
    # la probabilidad de que exista una adyacencia entre cualesquiera
    # dos vertices.     
    def init__random(self, n: int, p: float):
        for i in range(n):          # se crea y llena self.vertices
            vrt = self.Vertice()    # se crea un vertice nuevo para llenar vertices con n elementos
            self.vertices.append(vrt)
        for i in range(n):          # se crean las adyacencias aleatoriamente
            for j in range(i+1,n):
                if random.randint(0,100000)/100000 < p and not self.existe_ady(i, j):
                    self.vertices[i].adys.append(j)
                    self.vertices[j].adys.append(i)     
        return
    
    # REQ: n >> k >> ln n >> 1.
    # REQ: 0 <= beta <= 1.
    # Construye un grafo con n vertices, cada uno con k adyacencias EN PROMEDIO,
    # siguiendo el algoritmo de Watts-Strogatz:
    # https://en.wikipedia.org/wiki/Watts%E2%80%93Strogatz_model    
    def init__watts_strogatz(cls, n: int, k: int, ):
        return
    
    # Construye una red con base en la hilera grafo_json en formato json.
    def init__json(self, arch_grafo_json):
        #json_datos_grafo = open(arch_grafo_json).read()
        json_datos_grafo = arch_grafo_json.read()
        dic_datos_grafo = json.loads(json_datos_grafo)
        lista_dicts_vertices = list(dic_datos_grafo.values())[0]
        for i in range(len(lista_dicts_vertices)):
            vrt = self.Vertice()    # se crea un vertice nuevo para llenar vertices con la cantidad necesaria de elementos
            vrt.adys = lista_dicts_vertices[i].get('adys')
            self.vertices.append(vrt)            
        return
    
    # EFE: retorna true si 0 <= idvrt < n.
    # NOTA: idvrt significa "identificador de vertice".    
    def existe_vrt(self, idvrt: int):
        return 0 <= idvrt and idvrt < n
    
    # REQ: 0 <= idvrt_o < n and 0 <= idvrt_d < n.
    # EFE: retorna true si existe adyacencia entre los vertices idvrt_o e idvrt_d.   
    def existe_ady(self, idvrt_o: int, idvrt_d: int):
        return idvrt_d in self.vertices[idvrt_o].adys and idvrt_o in self.vertices[idvrt_d].adys
    
    # REQ: 0 <= idvrt < n.
    # EFE: retorna los idvrts de todos los vertices adyacentes a idVrt.
    def obt_idvrt_adys(self, idvrt: int):
        return copy.deepcopy(self.vertices[idvrt].adys)

    # REQ: 0 <= idvrt < n.
    # EFE: retorna el vertice con indice idvrt.
    # NOTA: implementa el operador [].
    def __getitem__(self, idvrt: int):
        return self.vertices[idvrt].vrt
    
    # EFE: retorna el total de vertices en el grafo.
    def obt_total_vrts(self):
        return len(self.vertices)
    
    # EFE: retorna el total de adyacencias en el grafo.
    def obt_total_adys(self):
        suma_adys = 0
        for i in range(len(self.vertices)):
            suma_adys += len(self.vertices[i].adys)
        return suma_adys
        
    def floyd_marshall(self):
        cant_vert = self.obt_total_vrts()
        mat_adyacencia = [ [np.inf for i in range(cant_vert)] for j in range(cant_vert) ]        
        
        for i in range(cant_vert):
            mat_adyacencia[i, i] = 0
        
        for i in range(cant_vert):
            for j in range(cant_vert):
                if self.existe_ady(i, j):
                    mat_adyacencia[i, j] = 1 
            
        for i in range(cant_vert):
            for j in range(cant_vert):
                for k in range(cant_vert):
                    if mat_adyacencia[i, j] > mat_adyacencia[i, k] + mat_adyacencia[k, j]:
                        mat_adyacencia[i, j] = mat_adyacencia[i, k] + mat_adyacencia[k, j]

# LECCIONES APRENDIDAS:
#1 los contenedores en python solo guardan referencias a objetos, igual que en JAVA,
#2 si se llena una lista con un objeto: lista = [objeto] * n, las n entradas de la lista apuntan al MISMO objeto,
#3 para referenciar "desde adentro de una clase" una clase embebida, se debe usar 'self': vrt = self.Vertice()
#4 el metodo especial __init__() NO SE PUEDE SOBRECARGAR, por tanto si se quiere agregar varios constructores
#5 tampoco se pueden agregar otros metodos que empiecen con __init,
#5 ---> es muy util el patron de Stav Shamir: https://stavshamir.github.io/python/2018/05/26/overloading-constructors-in-python.html
#6 ---> los metodos de clase deben ser precedidos por @classmethod
#7 ---> pero la idea de Stav Shamir no sirve si dos constructores tienen diferentes parametros,
#8 solo se me ocurrio crear un metodo __init__() sin parametros, un constructor basico y sendos inicializadores: init_json, init__watts_strogatz, etc.
#9 cualquier variable que se enuncie inmediatamente despues de class Xyz resulta ser una variable DE CLASE, no de instancia,
#10 para declarar variables de instancia se deben inicializar en __init__