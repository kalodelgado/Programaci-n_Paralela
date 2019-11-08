import numpy as np
import sys, getopt		## para obtener valores de linea de comandos

"""
EFE: parsea los valores m, n y s pasados al programa por línea de comando.
"""
def obt_argumentos(argv):
	k = 0 # cantidad de grupos a generar 
	m = 0 # cantidad de vectores
	n = 0 # la cantidad de dimensiones
	s = 0 # s o sigma es la raiz cuadrada de la desviación estándar
	try:
		## parsea la lista de parametros de consola
		## en opts queda una lista de pares (opcion, valor)
		## en args queda una lista de valores sin las opciones
		opts, args = getopt.getopt(argv,"h:k:m:n:s:")
	except getopt.GetoptError:
		print ('generador.py -k <cantidad de grupos> -m <cantidad de vectores> -n <cantidad de dimensiones> -s <valor de sigma>')
		sys.exit(2)
	for opt, arg in opts:
		if opt == '-h':
			print ('generador.py -k <cantidad de grupos> -m <cantidad de vectores> -n <cantidad de dimensiones> -s <valor de sigma>')
			sys.exit()
		elif opt in ("-k"):
			k = arg
		elif opt in ("-m"):
			m = arg
		elif opt in ("-n"):
			n = arg
		elif opt in ("-s"):
			s = arg
	return int(k), int(m), int(n), float(s)					# OJO: se convierten a ints

"""
EFE: genera un grupo de m vectores similares (con desviación estándar de s) de n dimensiones
	 con un factor de traslación ft con respecto al grupo anterior
"""
def gnr_grupo(ft, m, n, s):
	medias = ft + np.random.randn(1,n)			# genera el vector con las n medias de cada dimensión trasladadas según ft
	vectores = np.empty((0,m-1),float)			# genera una matriz sin filas de m-1 columnas porque al final agrega "medias"
	for i in range(0, n):
		fila_nueva = medias[0,i] + s * np.random.randn(1,m-1)	# genera una fila con m-1 valores
		vectores = np.append(vectores, fila_nueva, axis=0)	# agrega la fila a la matriz de vectores
	vectores = vectores.transpose()
	vectores = np.append(vectores, medias, axis=0 ) # se agrega el vector medias para verificar centroide
	return vectores

"""
EFE: genera un arreglo numpy que representa una partición pseudo-aleatoria de m en k
"""
def gnr_particion(k,m):
	prt = np.random.randint(int(m/(2*k)),int(m/k)+1,size = k) # genera un arreglo numpy con k valores x:int(m/(2*k)) <= x <  int(m/k) + 1
	a_distribuir = (m - np.sum(prt))/k						  # calcula cantidad a distribuir entre los k elementos de la partition
	with np.nditer(prt, op_flags=['readwrite']) as i:		  # define un iterador sobre prt con opción de lectura y escritura
		for x in i:											  # usa el iterador para recorrer prt y modificarlo con a_distribuir
			x[...] = x + a_distribuir
	remanente = (m - np.sum(prt))%k							  # calcula el remanente para completar m y lo suma al primer elemento
	prt[0] = prt[0] + remanente	
	return prt

def main(argv):
	k, m, n, s = obt_argumentos(argv)  			# obtiene valores de argumentos 
	vectores = np.empty((0,n),float)			# genera una matriz sin filas de n columnas
	particion = gnr_particion(k,m)				# genera la particion de m en k 
	factor_traslación = 0						# factor de traslación de los grupos que se generarán
	with np.nditer(particion) as itr:
		for x in itr:
			grupo_nuevo = gnr_grupo(factor_traslación, x, n, s) 	# genera un grupo nuevo con x elementos
			vectores = np.append(vectores, grupo_nuevo, axis = 0)	# agrega el nuevo grupo a vectores
			factor_traslación = factor_traslación + int(s*5)		# el factor de traslación se incrementa en 5 sigmas
	np.savetxt("particion.csv", particion, fmt = '%d')								# crea el archivo con la partición
	np.savetxt("vectores_ordenados.csv", vectores, fmt = '%5.2f',delimiter=",")		# crea el archivo a partir de los vectores
	np.random.shuffle(vectores)					# desordena los vectores
	np.savetxt("vectores_desordenados.csv", vectores, fmt = '%5.2f',delimiter=",")	# crea el archivo desordenado de los vectores
	return

	
if __name__ == "__main__": 	## implica que reading_argv está siendo ejecutado directamente y no por otro modulo
	main(sys.argv[1:])		## le pasa a main la lista de <opción, valor> de parametros obtenida del sistema