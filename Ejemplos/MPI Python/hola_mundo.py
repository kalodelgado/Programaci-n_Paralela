
from __future__ import print_function # importa desde __future__ la función de impresión
									  # de __future__ porque en algún momento no era parte del estandard
from mpi4py import MPI


comm = MPI.COMM_WORLD # Obtiene acceso al "comunicador"

print("Hola! soy el proceso %d de un total de %d  procesos en ejecucion..." % (comm.rank, comm.size))

comm.Barrier()   # obliga a todos los procesos a esperar que los demás hayan terminado antes de terminar

