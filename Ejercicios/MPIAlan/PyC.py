from mpi4py import MPI
import random
import sys, getopt #para recibir los parametros de consola

class Mensaje:
	def __init__(self, pe, pr):
		self.pidE = pe		## pid del proceso emisor
		self.pidR = pr		## pid del proceso receptor (sólo p0 actúa como receptor en este programa)
		self.msj = random.randint(0,100000) 	# mensaje es un entero generado al azar entre 0 y 100000
		return
		
	def toString(self):
		return str(self.pidE) + ':' + str(self.pidR) + ">" + str(self.msj) + '.'
		
def obt_valores_linea_comandos(argv):
	p_str = ""
	c_str = ""
	m_str = ""
	
	try:
		## parsea la lista de parametros de consola
		## en opts queda una lista de pares (opcion, valor)
		## en args queda una lista de valores sin las opciones
		opts, args = getopt.getopt(argv,"h:p:c:m:",["H=","P=","C=","M="])
	except getopt.GetoptError:
		print ('productor_consumidor.py -p <valor de P> -c <valor de C> -m <valor de M>')
		sys.exit(2)
	for opt, arg in opts:
		if opt == '-h':
			print ('productor_consumidor.py -p <valor de P> -c <valor de C> -m <valor de M>')
			sys.exit()
		elif opt in ("-p", "--P"):
			p_str = arg
		elif opt in ("-c", "--C"):
			c_str = arg
		elif opt in ("-m", "--M"):
			m_str = arg

	return int(p_str), int(c_str), int(m_str)
     
def main(argv):
    comm = MPI.COMM_WORLD
    pid = comm.rank	
    size = comm.size
    P = 0		# cantidad de Productores
    C = 0		# cantidad de Consumidores
    M = 0		#Cantidad tope de Mensajes

    if pid == 0:
        P, C, M = obt_valores_linea_comandos(argv)
        assert size == P + C
    P, C, M = comm.bcast((P,C,M),0)

    d = 0
    cantMsj = 0

    consumidores = [0] * C 
    sub_consumidores = [0] * C    
    if pid >= C:
        d = random.randint(0, C - 1)
        sub_consumidores[d] = 1
    comm.Barrier()

    for i in range(C):
        consumidores[i] = comm.allreduce(sub_consumidores[i], op=MPI.MAX)
    
    l_msj = []
    msjEnviados = 0
    
    if pid < C:		#Consumidor
        fin = True
        s = MPI.Status()        
        while consumidores[pid] and fin:
            comm.probe(source=MPI.ANY_SOURCE, tag=MPI.ANY_TAG, status=s)
            if s.tag == 0:
                msj = comm.recv(source=s.source, tag=0)
                l_msj.append(msj)
            if s.tag == 1:
                msj = comm.recv(source=s.source, tag=1)
                fin = False #Sacar del while
    else: 			#Productor
        cantMsj = random.randint(1, M)
        for i in range(cantMsj):
            msj = Mensaje(pid, d)
            comm.send(msj.toString(), dest=d, tag=0)
        f = "Termino " + str(pid) + ". Cantidad de msj enviados: " + str(cantMsj)
        comm.send(f, dest=d, tag=1) #Tag 1 = Termino el proceso
        f += ". Consumidor destino: " + str(d)
        print(f)

    comm.Barrier()

    if pid < C:
        if not consumidores[pid]:
            l_msj.append("No me seleccionaron como receptor.")
        nom_arch_msjs = str(pid) + ".txt"
        with open(nom_arch_msjs, 'w') as arch_mensajes:
            for msj in l_msj:
                arch_mensajes.write(str(msj)+'\n')
            arch_mensajes.close()
    
    comm.Barrier()
    return

if __name__ == "__main__": 	## implica que reading_argv está siendo ejecutado directamente y no por otro módulo
	main(sys.argv[1:])
