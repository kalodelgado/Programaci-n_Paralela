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
        
    start = calc_start(num_toss, pid, comm.size)
    end = calc_end(num_toss, pid, comm.size)
    
    num_circ = 0
    
    for i in range(start, end):
        x = np.random.uniform(-1, 1)
        y = np.random.uniform(-1, 1)
        x = x*x
        y = y*y
        if x+y <= 1:
            num_circ += 1
    
    global_sum = comm.reduce(num_circ, op=MPI.SUM)
    
    tiempo = MPI.Wtime() - tiempo
    
    if pid == 0:
        pi_est = 4 * global_sum/float(num_toss)
        print("Valor estimado de pi %.5f, Tiempo de ejecucion: %.5f" %(pi_est, tiempo))
        
    comm.Barrier()
    return

main()