from mpi4py import MPI
import random
import numpy as np

def calc_start(n, id, process_count):
    return int(n / process_count * id)

def calc_end(n, id, process_count):
    if id == process_count - 1:
        return n
        
    return int(n / process_count * (id + 1) )

def main():
    comm = MPI.COMM_WORLD    
    pid = comm.rank

    num_toss = 0

    if pid == 0:
        num_toss = int(input("Total de iteraciones: "))
    comm.Barrier()

    tiempo = MPI.Wtime()

    num_toss = comm.bcast(num_toss,0) #Todos pasan a tener el mismo valor de num_toss, obtenido por el cero
        
    if pid == 0:
        cont = 0
        cantP = 0 #Cuantos procesos han terminado
        s = MPI.Status()
        while (cantP < comm.size -1):
            comm.probe(source=MPI.ANY_SOURCE, tag=MPI.ANY_TAG, status=s)
            if s.tag == 1:
                msj = comm.recv(source=s.source, tag=1)
                print(msj)
                cont += 1
            else:
                msj = comm.recv(source=s.source, tag=0)
                print(msj)
                cantP += 1
    else:
        #tag 1 = aumenta el contador; tag 0 = terminé
        start = calc_start(num_toss, pid -1, comm.size -1)
        end = calc_end(num_toss, pid -1, comm.size -1)

        for i in range(start, end):
            x = np.random.uniform(-1, 1)
            y = np.random.uniform(-1, 1)
            x = x*x
            y = y*y
            if x+y <= 1:
                msj = "Process" + str(pid) + " send in iter " + str(i)
                comm.send(msj, dest=0, tag=1)
        msj = "Process" + str(pid) + " finished"
        comm.send(msj, dest=0, tag=0)
    
    tiempo = MPI.Wtime() - tiempo
    
    if pid == 0:
        pi_est = 4 * cont/float(num_toss)
        print("Valor estimado de pi %.5f, Tiempo de ejecucion: %.5f" %(pi_est, tiempo))
        
    comm.Barrier()
    return

main()