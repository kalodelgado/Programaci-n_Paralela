#Toma como nombre el nombre de la carpeta de donde se ejecuta
APPNAME=$(shell basename `pwd`)

CC=cc
CXX=g++
FLAGS=-g -Wall -Wextra
CFLAGS=$(FLAGS)
CXX=$(FLAGS)
LIBS=-pthread

#Que ejecutará el make.
all: $(APPNAME)

#Buscará un nombre(regla) en el archivo, al no existir sustituye el %
%: %.c 
	$(CC) $(FLAGS) $^ -o $@ $(LIBS) 
	# $^ = valor de la derecha; $@ = valor de la izquierda de la linea pasada 

%: %.cpp
	%(CXX) $(CXXFLAGS) $^ -o %@ $(LIBS)

#Para evitar 'actualizar' un ejecutable llamado clean
.PHONY: clean
clean:
	rm -f $(APPNAME) *.o	#-f = haga y no me informe

.PHONY: gitignore
gitignore: 
	echo $(APPNAME) > .gitignore
