#!/usr/bin/env python 

"""
Demuestra la multiplicación de una matriz por un vector ejecutando el producto iter veces:

  v(t+1) = M * v(t)

v es un vector numpy de longitud size y M una matriz de size*size
Se supone que size es multiplo de comm.size.

v se inicializa en ceros excepto v[0] = 1.0
M se inicializa en ceros excepto una diagonal M[i, i+1] = 1.0 para todo i.

Después de iter multiplicaciones:
v[iter] == 1 y v[i] == 0 para todo i diferente de iter.

Cada proceso es responsable de hacer una parte de la multiplicación de M*v(t),
es decir, sólo multiplica algunas de las filas por v. Luego cada proceso comunica
a todos los demás la parte del vector resultante que obtuvo. De esta manera cada
uno puede ejecutar la siguiente multiplicación hasta completar iter multiplicaciones.
"""

from __future__ import division
## numpy es un módulo especializado en cálculos numéricos, muy usado actualmente en ciencia de datos
## ver https://docs.scipy.org/doc/
import numpy as np 
from numpy.fft import fft2, ifft2
from math import ceil, fabs
from mpi4py import MPI
#from parutils import pprint, a partir de python 3 pprint aparece en un módulo propio que contiene la función pprint
import pprint ## tiene la ventaja de que es multi-entrante seguro

def main():
	size = 100           # longitud del vector v
	iter = 25            # cantidad de iteraciones a ejecutar
	comm = MPI.COMM_WORLD
	pprint.pprint("============================================================================")
	pprint.pprint(" Ejecutando %d procesos paralelos MPI " % comm.size)
	my_size = size // comm.size     	# *my_size* es el tamaño del vector que multiplica cada proceso
	assert size == comm.size*my_size	#  asegura que size es divisible por la cantidad de procesos
	my_offset = comm.rank*my_size		# calcula el punto de partida desde donde debe hacer el calculo en la matriz

	# crea el vector completo 1d de size columnas
	vec = np.zeros(size)	# todos los elementos son inicializados en cero...
	vec[0] = 1.0            #  ... excepto vec[0]

	# Crea localmente el trozo de la matriz a procesar: my_size filas, size columnas, inicializada en ceros
    # El for pone unos en la diagonal
	my_M = np.zeros((my_size, size))
	for i in range(my_size):
		j = (my_offset+i-1) % size	
		my_M[i,j] = 1.0 	# sólo estas posiciones-columna tendrán un 1.0, las demás quedan con ceros

	comm.Barrier()                    # sincroniza los procesos para capturar marca de tiempo-pared de inicial
	t_start = MPI.Wtime()

	for t in range(iter):				## se ejecutaran iter (ver l#46) repeticiones para lograr que en vec[iter] quede un 1.0
		my_new_vec = np.inner(my_M, vec)## np.inner multiplica my_M por vec: https://docs.scipy.org/doc/numpy/reference/generated/numpy.inner.html 
		# todos los procesos obtienen de los demas en vec lo que cada uno deja en my_new_vec
		# cada proceso aporta un vector con parte del vector final del producto
		comm.Allgather([my_new_vec, MPI.DOUBLE],[vec, MPI.DOUBLE]) 	# OJO para numpy se deben usar funciones con mayúscula
        #Une los vectores peq (my_new_vec) al vec de tamaño size en orden de rank(id de proceso)
		
	comm.Barrier()						# nuevamente sincroniza para tomar marca de tiempo final
	t_diff = MPI.Wtime() - t_start    	# Sincroniza para capturar marca de tiempo-pared de fin ###

	if fabs(vec[iter]-1.0) > 0.:			# verifica secuencia de productos
		pprint("!! Error: resultado equivocado!")

	pprint.pprint(" %d iteraciones de tamaño %d en %5.2fs: %5.2f iteraciones por segunds" % (iter, size, t_diff, iter/t_diff))
	pprint.pprint("============================================================================")
	if comm.rank == 0:
		print(vec)
	return

main()