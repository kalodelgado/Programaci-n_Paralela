from mpi4py import MPI
import random
import numpy as np
import math

def calc_start(n, id, process_count):
    return int(n / process_count * id)

def calc_end(n, id, process_count):
    if id == process_count - 1:
        return n + 1
        
    return int(n / process_count * (id + 1) )

def goldbach(numero):
    rango = int(numero/3 + 1)
    c = 0
    for a in range(2, rango):
        if isPrime(a):
            for b in range(a, numero):
                if isPrime(b):
                    c = numero - a - b
                    if c >= b and isPrime(c):
                        msj = str(numero) + ":" + str(a) + "+" + str(b) + "+" + str(c)
                        break
    return msj 

def isPrime(numero):
    if numero == 2:
        return True
    if numero % 2 == 0:
        return False

    sqrn = int( math.sqrt(numero) + 1) #Raiz cuadrada del numero
    for i in range(3, sqrn):
        if numero % i == 0:
            return False

    return True

def main():
    comm = MPI.COMM_WORLD
    pid = comm.rank
	
    num = 0
    
    if pid == 0:
        num = int(input("Numero limite: "))
        num -= 6
    comm.Barrier()

    tiempo = MPI.Wtime()
    num = comm.bcast(num, 0)
    datos = [] * num
	
    start = 6 + calc_start( num, comm.rank, comm.size)
    end = 6 + calc_end( num, comm.rank, comm.size)
	
    my_datos = [] * (end-start)   
    for i in range(start, end):
        msj = goldbach(i)
        my_datos.append(str(msj))
    comm.Barrier()
	
    datos = comm.gather(my_datos, 0)

    tiempo = MPI.Wtime() - tiempo
	
    if pid == 0:
        print("Tiempo de ejecucion: %.5f \n" %(tiempo))
        print(datos)
    comm.Barrier()
    return 

main()