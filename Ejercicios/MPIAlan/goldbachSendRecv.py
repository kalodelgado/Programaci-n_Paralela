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

    l_msj = [] #lista de mensajes con las sumas para el proces0

    num = 0    
    if pid == 0:
        num = int(input("Numero limite: "))
        num -= 6
    comm.Barrier()

    tiempo = MPI.Wtime()
    num = comm.bcast(num, 0)

    if pid == 0:
        cantP = 0 #Cuantos procesos han terminado
        s = MPI.Status()
        while (cantP < comm.size -1):
            comm.probe(source=MPI.ANY_SOURCE, tag=MPI.ANY_TAG, status=s)
            if s.tag == 1:
                msj = comm.recv(source=s.source, tag=1)
                l_msj.append(msj)
            else:
                msj = comm.recv(source=s.source, tag=0)
                cantP += 1
    else:
        start = 6 + calc_start( num, comm.rank -1, comm.size -1)
        end = 6 + calc_end( num, comm.rank -1, comm.size -1)
        for i in range(start, end):
            msj = goldbach(i)
            comm.send(msj, dest=0, tag=1)
        comm.send(msj, dest=0, tag=0) #Tag 0, process finished

    tiempo = MPI.Wtime() - tiempo

    if pid == 0:
        print("Tiempo de ejecucion: %.5f \n" %(tiempo))

        nom_arch_msjs = "Sumas de Goldbach.txt"
        with open(nom_arch_msjs, 'w') as arch_mensajes:
            for msj in l_msj:
                arch_mensajes.write(str(msj)+'\n')
            arch_mensajes.close()

    comm.Barrier()
    return 

main()