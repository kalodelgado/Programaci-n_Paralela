all: ejec	#Que ejecutará el make

ejec: hello_i_of_w.c
	cc -g -Wall -Wextra hello_ordered_wait.c -o ejec -pthread
	
.PHONY: clean
clean:
	rm -f ejec *.o	#-f = haga y no me informe
