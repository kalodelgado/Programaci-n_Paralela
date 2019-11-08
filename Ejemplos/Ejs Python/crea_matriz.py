#!/usr/bin/env python 

"""
Genera una matriz m de cf x cc usando dos formas distintas:
1. con listas de listas
2. con arreglos de arreglos
"""

import array as arrays
import random
import math

def main():
	cf = int(input("# filas: "))
	cc = int(input("# columnas: "))
	
	## usando listas de listas:
	m1 = [[0 for i in range(cc)] for j in range(cf)]
	print("Matriz como lista de listas: ")
	print(m1)
	
	## ahora usando una list de array:
	fila = arrays.array('i',[0 for i in range(cc)])
	m2 = [ fila for j in range(cf)]
	print("Matriz como lista de array")
	print(m2)
	print("NO ES POSIBLE DEFINIR UNA MATRIZ COMO UN array de arrays porque array sólo puede contener tipos básicos")
	return
	
main()