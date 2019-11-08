#!/usr/bin/env python 

from __future__ import division
from __future__ import print_function

import numpy as np 
from mpi4py import MPI
from time import time
import sys, getopt		## para obtener valores de linea de comandos

"""
EFE: parsea los valores l, m y n pasados al programa por linea de comando.
"""
def obt_valores_linea_comandos(argv):
	l = 0
	m = 0
	n = 0
	try:
		## parsea la lista de parametros de consola
		## en opts queda una lista de pares (opcion, valor)
		## en args queda una lista de valores sin las opciones
		opts, args = getopt.getopt(argv,"h:l:m:n:")
	except getopt.GetoptError:
		print ('matriz_x_matriz_numpy.py -l <# filas A> -m <# columnas A y filas B> -n <# columnas B># ')
		sys.exit(2)
	for opt, arg in opts:
		if opt == '-h':
			print ('matriz_x_matriz_numpy.py -l <# filas A> -m <# columnas A y filas B> -n <# columnas B># ')
			sys.exit()
		elif opt in ("-l"):
			l = arg
		elif opt in ("-m"):
			m = arg
		elif opt in ("-n"):
			n = arg
	return int(l), int(m), int(n)					# OJO: se convierten a ints

"""
Multiplica dos matrices A x B retornando una matriz nueva.
A es de l x m, B es de m x n, el resultado será l x n. 
Supone que l % comm.size == 0
"""
def main(argv):
	comm = MPI.COMM_WORLD
	pid = comm.rank
	dims = []		# todos ocupan la cantidad de filas de A que les toca, la cantidad de columnas de A y filas de B y columnas de B

	if pid == 0:
		L,M,N = obt_valores_linea_comandos(argv) #
		assert L % comm.size == 0
		dims.append(int(L/comm.size)) 	# cantidad de filas que le toca a cada proceso
		dims.append(M)				# cantidad de columnas de A y filas de B
		dims.append(N)				# cantidad de columnas de B
	dims = comm.bcast(dims,0)		# p0 comunica las dimensiones a todos
	A = None 						# todos la ocupan para el Scatter
	B = np.zeros((dims[1], dims[2]))	# todos la ocupan para multiplicar
	C = np.zeros((dims[0]*comm.size, dims[2]))	# todos la ocupan para recibir en el comm.Gather, AUNQUE NO RECIBAN NADA
	mi_A = np.zeros((dims[0], dims[1]))	# todos se preparan para recibir parte de A
	mi_C = np.zeros((dims[0], dims[2]))	# todos la usan para guardar el resultado parcial que calculan
	
	# p0 crea las matrices A y B iniciándolas al azar con random.normal 
	# https://docs.scipy.org/doc/numpy/reference/generated/numpy.random.normal.html
	if pid == 0:
		A = np.random.normal(size=(L, M)).astype(np.float32)
		B = np.random.normal(size=(M, N)).astype(np.float32)
	
	comm.Scatter(A, mi_A, 0)		# todos reciben su parte
	comm.Bcast(B, 0)				# todos reciben B
	
	mi_C = np.inner(mi_A, B) 	# https://docs.scipy.org/doc/numpy/reference/generated/numpy.inner.html
	
	comm.Gather(mi_C, C, 0)		# p0 recoge los resultados parciales en C
	if pid == 0:
		print(C)
	return

if __name__ == "__main__": 	## implica que reading_argv está siendo ejecutado directamente y no por otro modulo
	main(sys.argv[1:])		## le pasa a main la lista de <opción, valor> de parametros obtenida del sistema


