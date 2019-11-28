from mpi4py import MPI

def calc_start(n, id, process_count):
    return int(n / process_count * id)

def calc_end(n, id, process_count):
    if id == process_count - 1:
        return n
        
    return int(n / process_count * (id + 1) )
	
def main():
	comm = MPI.COMM_WORLD
	pid = comm.rank
	size = comm.size
	
	list = ['a','b','c','d','e','f']
	
	start = calc_start(len(list), pid, size)
	end = calc_end(len(list), pid, size)
	
	my_list = []
	for i in range(start,end):
		for j in range(len(list)):
			if j != i:
				msj = str(list[i]) + str(list[j])
				my_list.append(msj)
	
	flist = comm.reduce(my_list,op=MPI.SUM)
	
	if pid == 0:
		print(flist)
	
	return

main()