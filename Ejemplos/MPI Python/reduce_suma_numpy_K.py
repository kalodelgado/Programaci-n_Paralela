 #!/usr/bin/env python 

"""
p0 recibe un arreglo numpy con los valores aportados por todos los procesos y la reducción por MPI.SUM de todos los valores.
Los valores varían entre min y max aportados por el usuario por linea de comandos.
"""
from mpi4py import MPI
import random
import sys, getopt		## para obtener valores de linea de comandos
import numpy as np

"""
EFE: parsea los valores min y max pasados al programa por linea de comando, tal como debe hacerse cuando se use un cluster.
"""
def obt_valores_linea_comandos(argv):
	min = 0
	max = 0
	k = 0
	try:
		## parsea la lista de parametros de consola
		## en opts queda una lista de pares (opcion, valor)
		## en args queda una lista de valores sin las opciones
		opts, args = getopt.getopt(argv,"h:m:x:k:",["min=","max=","k="])
	except getopt.GetoptError:
		print ('reduce_suma.py -m <valor float minimo> -x <valor float maximo> -k <valor dimension>')
		sys.exit(2)
	for opt, arg in opts:
		if opt == '-h':
			print ('reduce_suma.py -m <valor float minimo> -x <valor float maximo> -k <valor dimension>')
			sys.exit()
		elif opt in ("-m", "--min"):
			min = arg
		elif opt in ("-x", "--max"):
			max = arg
		elif opt in ("-k", "--k"):
			k = arg
	return float(min), float(max), int(k)					# OJO: se convierten a floats
	
def main(argv):
	comm = MPI.COMM_WORLD
	pid = comm.rank
	lista = np.zeros(comm.size, dtype='float64')	# se crea un arreglo numpy de comm.size posiciones inicializado en ceros
	min = 0.0
	max = 0.0
	k = 0
	if pid == 0:
		min, max, k = obt_valores_linea_comandos(argv)
	min,max,k = comm.bcast((min,max,k),0)				# p0 comunica a todos el par de limites para rango de numeros
	global_sum = np.zeros(k, dtype='float64')		# se crea un arreglo numpy de una posicion inicializado en ceros
	x = np.random.uniform(min, max, k)							# comm.Reduce recibe solo arreglos, con k posiciones
	comm.Reduce(x, global_sum, op=MPI.SUM)			# ambos x y global_sum DEBEN ser arreglos (listas o tuples segun doc)
	if pid == 0:
		print("La suma de los numeros es: ", global_sum)
	lista = comm.gather(x, 0)						# ambos x y lista DEBEN ser arreglos (listas o tuples segun doc)
	if pid == 0:
		print("La lista de numeros es: ", lista)
	return

if __name__ == "__main__": 	## implica que reading_argv está siendo ejecutado directamente y no por otro modulo
   main(sys.argv[1:])		## le pasa a main la lista de opciones, parametros obtenida del sistema
   
"""
LISTA DE OPERADORES PARA REDUCCION:
MPI.MIN        minimum
MPI.MAX        maximum
MPI.SUM        sum
MPI.PROD       product
MPI.LAND       logical and
MPI.BAND       bitwise and
MPI.LOR        logical or
MPI.BOR        bitwise or
MPI.LXOR       logical xor
MPI.BXOR       bitwise xor
MPI.MAXLOC     max value and location
MPI.MINLOC     min value and location
"""