APPNAME=$(shell basename `pwd`)

#Que ejecutará el make.
all: $(APPNAME)

#Buscará un nombre(regla) en el archivo, al no existir sustituye el %
%: %.c 
	cc -g -Wall -Wextra $^ -o $@ -pthread 
	# $^ = valor de la derecha; $@ = valor de la izquierda de la linea pasada 

#Para evitar 'actualizar' un ejecutable llamado clean
.PHONY: clean
clean:
	rm -f $(APPNAME) *.o	#-f = haga y no me informe
