#!/usr/bin/python3
from mpi4py import MPI
import random
import math
import numpy as np
import matplotlib.pyplot as plt

def calc_start(n, id, process_count):
	return int(n / process_count * id)

def calc_end(n, id, process_count):
	if id == process_count - 1:
		return n
		
	return int(n / process_count * (id + 1) )

#-------------------------------------------------------------------------
# Function:  gen_data
# Purpose:   Generate random doubles in the range min_meas <= x < max_meas
# In args:   min_meas:    the minimum possible value for the data
           #max_meas:    the maximum possible value for the data
           #start and end:  the number of measurements
# Out arg:   data:        the actual measurements
def gen_data(data, min_meas, max_meas, data_count):
    random.seed()
    for i in range(data_count):
        new_data = random.uniform(min_meas, max_meas)
        data.append(new_data)
    return data

#---------------------------------------------------------------------
# Function:  gen_bins
# Purpose:   Compute max value for each bin, and store 0 as the
#            number of values in each bin
# In args:   min_meas:   the minimum possible measurement
#            max_meas:   the maximum possible measurement
#            bin_count:  the number of bins
# Out args:  bin_maxes:  the maximum possible value for each bin
def gen_bins(min_meas, max_meas, bin_count):
    bin_maxes = []
    bin_counts = []
    bin_width = (max_meas - min_meas) / bin_count
    for i in range(bin_count):
        bin_maxes.append(min_meas + (i + 1)*bin_width)
        bin_counts.append(0)
    return bin_maxes, bin_counts

#---------------------------------------------------------------------
# Function:  which_bin
# Purpose:   Use binary search to determine which bin a measurement
#            belongs to
# In args:   data:       the current measurement
#            bin_maxes:  list of max bin values
#            bin_count:  number of bins
#            min_meas:   the minimum possible measurement
# Return:    the number of the bin to which data belongs
# Notes:
# 1.  The bin to which data belongs satisfies
#
#            bin_maxes[i-1] <= data < bin_maxes[i]
#     where, bin_maxes[-1] = min_meas
# 2.  If the search fails, the function prints a message and exits
def which_bin(data, bin_maxes, bin_count, min_meas):
    bottom = 0
    top = bin_count - 1
    while bottom <= top:
        mid = int((bottom + top) / 2)
        bin_max = bin_maxes[mid]
        if mid == 0:
            bin_min = min_meas
        else:
            bin_min = bin_maxes[mid - 1]
        if data >= bin_max:
            bottom = mid + 1
        else: 
            if data < bin_min:
                top = mid - 1
            else:
                return mid
    return 0

#---------------------------------------------------------------------
# Function:  count_data_per_bin
# Purpose:   Count the number of elements in each bin.
# In args:   min_meas: the minimum possible measurement
#	     bin_maxes: the max value for each bin
#            bin_count:  number of bins
#            data_count: the number of measurements
#	     data: the current measurements
#            bin_counts: the number of data values in each bin
# Out args:  bin_counts:  the number of elements in each bin
def count_data_per_bin(min_meas, bin_maxes, bin_count, data_count, data, bin_counts, start, end):
    for i in range(start, end):
        bin_counts[which_bin(data[i], bin_maxes, bin_count, min_meas)] += 1
    return bin_counts

#---------------------------------------------------------------------
# Function:  print_histogram
# Purpose:   Print a histogram.  The number of elements in each
#            bin is shown by a bar of corresponding length.
# In args:   
#	     bin_counts:  the number of elements in each bin    
#            bin_count:   the number of bins
def print_histogram(bin_counts, bin_count):    
    ind = np.arange(bin_count)                  # la localizacion en X de las barras
    width = 0.5                                 # el ancho de las barras
    barras = plt.bar(ind, bin_counts, width)    # crea las barras con los conteos de rangos
    plt.xlabel('Rangos')                        # titulo del eje X
    plt.xticks(ind)                             # valores en el eje X
    plt.ylabel('Frecuencias')                   # titulo del eje Y
    plt.title('Histograma')                     # titulo del grafico
    plt.show()                                  # grafica
    return
    
def main():
    comm = MPI.COMM_WORLD
    pid = comm.rank

    #Se declaran las variables para posterior broadcast
    bin_count = 0
    min_meas = 0
    max_meas = 0
    data_count = 0
    bin_maxes = 0    
    bin_maxes = []
    bin_counts = []
    data = []

    if pid == 0:
        bin_count = int(input("Cantidad de rangos o barras: "))
        min_meas = int(input("Minimo del rango: "))
        max_meas = int(input("Maximo del rango: "))
        data_count = int(input("Cantidad de datos: "))
        bin_maxes, bin_counts = gen_bins(min_meas, max_meas, bin_count)
        data = gen_data(data, min_meas, max_meas, data_count)
    comm.Barrier()

    tiempo = MPI.Wtime()

    bin_count, min_meas, max_meas, data_count, bin_maxes, bin_counts, data = comm.bcast( (bin_count, min_meas, max_meas, data_count, bin_maxes, bin_counts, data) , 0)
	
    if pid == 0:        
        print ("Cantidad de datos: ",len(data))
        print ("Datos: ", data)
        print ("Rangos: ", bin_maxes)
        for i in range(data_count):
            print("Valor: ", str(data[i]), " en ", str( which_bin( data[i], bin_maxes, bin_count, min_meas) ) )

    start = calc_start(len(data), pid, comm.size)
    end = calc_end(len(data), pid, comm.size)

    bin_counts = count_data_per_bin(min_meas, bin_maxes, bin_count, data_count, data, bin_counts, start, end) #P

    comm.Barrier()

    all_bin_counts = np.zeros(bin_count)
    for i in range( len(bin_counts) ):
        all_bin_counts[i] = comm.reduce(bin_counts[i], op=MPI.SUM)

    tiempo = MPI.Wtime() - tiempo
    if pid == 0:
        print ("Datos por rango: ", bin_counts)
        print("Tiempo de ejecucion: %.5f \n" %(tiempo))
        print ("Conteo de rangos: ", all_bin_counts)
        print_histogram(all_bin_counts, bin_count)

    comm.Barrier()    
main() # ejecuta la funcion main()
