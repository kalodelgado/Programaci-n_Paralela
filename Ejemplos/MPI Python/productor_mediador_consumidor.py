""" Este programa ilustra el patrón Productor-Consumidor con MPI4PY.
	Cada uno de los P productores envía N mensajes a p0 que los encola.
	Los C consumidores envían un mensaje a p0 cuando están listos para recibir otro mensaje.
	Cuando los consumidores reciben un mensaje de p0, lo escriben en un archivo de texto denominado msj_##.txt
	donde ## corresponde al id del proceso consumidor.
	Tanto P, C y N son provistos por el usuario por línea de comando en la consola.
	Los procesos con id de 1 a P serán los productores.
	Los procesos con id de P+1 a P+C serán los consumidores.
	Por tanto se requiere que MPI.COMM_WORLD.size == 1 + P + C
""" 

import pprint ## tiene la ventaja de que es multi-entrante seguro
import random
import sys, getopt
from mpi4py import MPI
from queue import Queue

class Mensaje:
	def __init__(self, pe, pr):
		self.pidE = pe		## pid del proceso emisor
		self.pidR = pr		## pid del proceso receptor (sólo p0 actúa como receptor en este programa)
		self.msj = random.randint(0,100000) 	# mensaje es un entero generado al azar entre 0 y 100000
		return
		
	def toString(self):
		return str(self.pidE) + ':' + str(self.pidR) + ">" + str(self.msj) + '.'
		
"""
EFE: parsea los valores de P, C y N que se le pasan al programa por línea de comando.
"""
def obt_valores_linea_comandos(argv):
	p_str = ""
	c_str = ""
	n_str = ""
	
	try:
		## parsea la lista de parametros de consola
		## en opts queda una lista de pares (opcion, valor)
		## en args queda una lista de valores sin las opciones
		opts, args = getopt.getopt(argv,"h:p:c:n:",["H=","P=","C=","N="])
	except getopt.GetoptError:
		print ('productor_consumidor.py -p <valor de P> -c <valor de C> -n <valor de N>')
		sys.exit(2)
	for opt, arg in opts:
		if opt == '-h':
			print ('productor_consumidor.py -p <valor de P> -c <valor de C> -n <valor de N>')
			sys.exit()
		elif opt in ("-p", "--P"):
			p_str = arg
		elif opt in ("-c", "--C"):
			c_str = arg
		elif opt in ("-n", "--N"):
			n_str = arg

	return int(p_str), int(c_str), int(n_str)
	
def main(argv):
	comm = MPI.COMM_WORLD # Obtiene acceso al "comunicador"
	pid = comm.rank
	size = comm.size
	P = 0		# cantidad de Productores
	C = 0		# cantidad de Consumidores
	N = 0		# cantidad de mensajes que enviará cada Productor a p0
	l_msjs = []	# lista de mensajes recibidos de p0 por parte de los consumidores, ni p0 ni los productores la usan.
	
	# p0 saca los valores de los parámetros de consola y valida P y C luego los comunica a todos:
	if pid == 0:
		P, C, N = obt_valores_linea_comandos(argv)
		assert size == 1 + P + C
	P, C, N = comm.bcast((P,C,N),0) 	# todos ocupan saber si les tocó el rol de Productores o de Consumidores

	# comienza el procesamiento de los mensajes
	# los procesos se envían dos tipos de mensajes: con tag == 0 o tag == 1
	# los mensajes con tag == 0 son mensajes de datos, es decir instancias de Mensaje.
	# los mensajes con tag == 1 son mensajes de control: 
	# para p0 con tag == 1: dato == 0 ==> no hay nada en cola, dato == 1 ==>  sí hay Mensajes en cola, dato == -1 ==> se acabaron todos los mensajes
	# para p0 con tag == 0 ==> que se está enviando un Mensaje
	# los consumidores sólo envían: dato == 1 tag == 1 ==> esperando mensajes
	# para los productores sólo envían: Mensaje y tag == 0 ==> enviando un Mensaje

	if pid == 0:
		cm = 0		# contador de mensajes en cero
		cpt = 0 	# contador de productores que terminaron
		cola_msjs = Queue() # cola de mensajes de datos
		"""
		s contiene dos datos importantes del mensaje que se está recibiendo:
		s.source el proceso emisor
		s.tag el tipo de mensaje que se está recibiendo
        s.count tamaño del mensaje
		"""		
		s = MPI.Status()	# provee información sobre el mensaje que se va a recibir
		comm.probe(source=MPI.ANY_SOURCE, tag=MPI.ANY_TAG, status=s) # espera
		# mientras no se hayan terminado de enviar todos los mensajes 
		while (cm < N * P):		
			pprint.pprint("p0 R de %d con: tag %d" % (s.source, s.tag))
			if s.tag == 1:		# recibe una solicitud de un consumidor
				dato = comm.recv(source=s.source,tag=1)	# se saca el mensaje de la cola del COMUNICADOR de MPI
				pprint.pprint("p0 R de %d con: tag %d y dato %d" % (s.source, s.tag, dato))
				if not cola_msjs.empty():
					# envía un mensaje de datos a s.source que lo pidió y lo saca de la cola_msjs
					msj = cola_msjs.get()					# saca un mensaje de la cola para enviarlo al C s.source
					comm.send(msj, dest=s.source, tag=0)	# luego le manda el Mensaje
					pprint.pprint("p0 E a C %d con: msj %s" % (s.source, msj.toString()))
					cm = cm + 1
				else:
					comm.send(0, dest=s.source, tag=1)	# envía un 0 indicando a s.source que la cola está vacía de momento
					pprint.pprint("p0 E a C %d con: dato == 0  tag 1"%(s.source))
			else:				# s.tag == 0 ==> que está recibiendo un mensaje de datos del productor s.source
				# saca de la cola del COMUNICADOR de MPI el mensaje de datos de s.source y lo encola en cola_msjs
				msj = comm.recv(source=s.source, tag=0)	
				pprint.pprint("p0 encola "+msj.toString())
				cola_msjs.put(msj)
			comm.probe(source=MPI.ANY_SOURCE, tag=MPI.ANY_TAG, status=s)	# espera
		for d in range(P+1,P+C+1):
			comm.isend(-1, dest=d, tag=1)	# envía un -1 indicando a s.source que se acabaron todos los mensajes
			pprint.pprint("p0 E a C %d con: dato -1 tag 1"%(d))
		pprint.pprint("p0 terminO!! con mensajes de %d Productores"%(cpt))
		
	if (1 <= pid) and (pid <= P): # le tocó ser Productor
		pprint.pprint("Proceso %d juega rol de Productor" % (pid))
		for i in range (N):
			msj = Mensaje(pid, 0)
			comm.send(msj, dest=0, tag=0)	# el proceso en un mensaje a p0 y espera a que p0 reciba el mensaje
			pprint.pprint("P %d E a p0 con: mensaje %s"%(pid,msj.toString()))
		pprint.pprint("P%d terminO!!"%(pid))

	if P < pid:					  # le tocó ser Consumidor
		pprint.pprint("proceso %d juega rol de Consumidor" % (pid))
		fin = False
		s = MPI.Status()	# provee información sobre el mensaje que se va a recibir
		comm.send(1, dest=0, tag=1)		# pide a p0 algún mensaje en cola, espera a que p0 reciba el mensaje
		pprint.pprint("C %d E a p0 con: dato == 1 y tag == 1 "%(pid))
		comm.probe(source=0, tag=MPI.ANY_TAG, status=s) #espera respuesta
		while not fin:
			if s.tag == 1:						# recibe de p0 un mensaje de control
				dato = comm.recv(source=0, tag=1)
				if dato == -1:					# p0 ya no tiene más mensajes en cola
					fin = True					# se terminó el envío de mensajes
					pprint.pprint("C%d terminO!!"%(pid))
					nom_arch_msjs = "msjs_" + str(pid) + ".txt"
					with open(nom_arch_msjs, 'w') as arch_mensajes:
						for msj in l_msjs:
							arch_mensajes.write(msj.toString()+'\n')
						arch_mensajes.close()
				else:	# de momento p0 no tiene mensajes, pero no se ha terminado, debe seguir esperando
					comm.isend(1, dest=0, tag=1)		# pide a p0 algún mensaje en cola, NO espera a que p0 reciba el mensaje
					pprint.pprint("C %d E a p0 con: dato == 1 y tag == 1 "%(pid))
					comm.probe(source=0, tag=MPI.ANY_TAG, status=s) #espera respuesta
			else:
				msj = comm.recv(source = 0, tag=0)	# espera a que p0 le envíe el mensaje
				pprint.pprint("C %d archiva mensaje %s"%(pid,msj.toString()))
				l_msjs.append(msj)				# pone el mensaje en una lista para luego generar un archivo
				comm.isend(1, dest=0, tag=1)		# pide a p0 otro mensaje, NO espera a que p0 reciba la solicitud
				pprint.pprint("C %d E a p0 con: dato == 1 y tag == 1 "%(pid))
				comm.probe(source=0, tag=MPI.ANY_TAG, status=s) #espera respuesta
		pprint.pprint("C%d terminO!!"%(pid))

	return

if __name__ == "__main__": 	## implica que reading_argv está siendo ejecutado directamente y no por otro módulo
	main(sys.argv[1:])		## le pasa a main la lista de opciones, parametros obtenida del sistema
"""
Intentar para analizar el efecto:
1. usar en L#91 iprobe en lugar de probe
2. usar en L#105 isend en lugar de send

Variantes útiles de este programa
1. Que los Productores generen una cantidad al azar de mensajes y terminen indicando que lo han hecho a p0 
para que p0 registre la finalización de cada Productor conforme se va dando.
"""