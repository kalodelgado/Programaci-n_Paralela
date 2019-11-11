from mpi4py import MPI
import random
import math

def calc_start(n, id, process_count):
    return int(n / process_count * id)

def calc_end(n, id, process_count):
    if id == process_count - 1:
        return n + 1
        
    return int(n / process_count * (id + 1) )
	
def calc_primos(num):
    l_primos = []

    for i in range(2, num):
        primo = True
        contador = 2

        sqrn = int( math.sqrt(i) )
        while (contador <= sqrn) and primo:
            if i % contador == 0:
                primo = False

            contador += 1

        if primo:
            l_primos.append(i)
    return l_primos

def descomp_prima(numero, l_primos):
    index = 0
    factores = []

    while int(l_primos[index]) <= numero:
        if (numero % l_primos[index]) == 0:
            factores.append(l_primos[index])
            numero /= l_primos[index]
        else:
            index += 1 #Incrementamos indice y long de lista		
            
    return factores
	
def main():
    comm = MPI.COMM_WORLD
    pid = comm.rank

    num = 0
    l_primos = []

    if pid == 0:
        num = int(input("Numero limite: "))
        l_primos = calc_primos(num)
        num -= 2
    comm.Barrier()

    tiempo = MPI.Wtime()
    num, l_primos = comm.bcast( (num, l_primos), 0)
    l_msj = []
    
    if pid < comm.size / 2:
        fin = True
        s = MPI.Status()
        while fin:
            comm.probe(source=MPI.ANY_SOURCE, tag=MPI.ANY_TAG, status=s)
            if s.tag == 0:
                msj = comm.recv(source=s.source, tag=0)
                l_msj.append(msj)
            else:
                msj = comm.recv(source=s.source, tag=1) #tag = 1 = terminó
                fin = False
    else:
        P = (comm.size / 2)
        start = 2 + calc_start( num, comm.rank - P, comm.size - P )
        end = 2 + calc_end( num, comm.rank - P, comm.size - P )
       
        for i in range(start, end):
            msj = "P" + str(pid) + "; "+ str(i) + ": "
            factores = descomp_prima(i, l_primos)

            for j in range( len(factores) ):
                cfactor = factores.count(factores[j]) #cantidad de elementos iguales a f[j]
                msj += str(factores[j]) + "(" + str(cfactor) + ")"
                if j != len(factores) - 1:
                    msj += " * "
                j += cfactor
            comm.send(msj, dest= pid - P, tag=0)
        comm.send(msj, dest=pid - P, tag=1)

    tiempo = MPI.Wtime() - tiempo
    
    if pid == 0:
        print("Tiempo de ejecucion: %.5f \n" %(tiempo))
    comm.Barrier()
    
    if pid < comm.size / 2:
        nom_arch_msjs = str(pid) + " Descomposion prima.txt"
        with open(nom_arch_msjs, 'w') as arch_mensajes:
            for msj in l_msj:
                arch_mensajes.write(str(msj)+'\n')
            arch_mensajes.close()
    comm.Barrier()
    return
	
main()