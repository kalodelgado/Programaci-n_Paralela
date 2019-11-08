"""
Los N procesos se pasan un mensaje uno a otro formando un anillo que empieza y termina en p0.
p0 --> p1 --> p2 --> ... --> pN --> p0
"""

import pprint ## tiene la ventaja de que es multi-entrante seguro
import random
from mpi4py import MPI
	
class Mensaje:
	def __init__(self, pe, pr):
		self.pidE = pe		## pid del proceso emisor
		self.pidR = pr		## pid del proceso receptor (sólo p0 actúa como receptor en este programa)
		self.msj = random.randint(0,100000) 	# mensaje es un entero generado al azar entre 0 y 100000
		return
		
	def toString(self):
		return str(self.pidE) + ':' + str(self.pidR) + ">" + str(self.msj) + '.'
		
def main():
	comm = MPI.COMM_WORLD # Obtiene acceso al "comunicador"
	pid = comm.rank
	N = comm.size
	
	if pid == 0:
		msje = Mensaje(0,1)
		comm.send(msje, dest=1)	# envía msje a p1
		pprint.pprint("0 E %s a 1"%(msje.toString()))
		msjr = comm.recv(source=N-1)
		pprint.pprint("0 R %s de %d"%(msjr.toString(),N-1))
	elif pid < N - 1:
		msjr = comm.recv(source=pid-1)
		pprint.pprint("%d R %s de %d"%(pid, msjr.toString(), pid-1))
		comm.send(msjr, dest=pid+1)	# envía msje a pid+1
		pprint.pprint("%d E %s a %d"%(pid, msjr.toString(), pid+1))
	else:
		msjr = comm.recv(source=pid-1)
		pprint.pprint("%d R %s de %d"%(pid, msjr.toString(), pid-1))
		comm.send(msjr, dest=0)	# envía msje a p0
		pprint.pprint("%d E %s a %d"%(pid, msjr.toString(), 0))
	return
	
main()