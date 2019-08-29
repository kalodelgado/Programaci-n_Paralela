#Toma como nombre el nombre de la carpeta de donde se ejecuta
APPNAME=$(shell basename `pwd`)

CC=mpicc
CXX=mpic++
FLAGS=-g -Wall -Wextra -std=c11 -fopenmp -lm -std=c99
CFLAGS=$(FLAGS)
CXX=$(FLAGS)
LIBS=

#Que ejecutará el make.
all: $(APPNAME)

#Buscará un nombre(regla) en el archivo, al no existir sustituye el %
%: %.c 
	$(CC) $(FLAGS) $^ -o $@ $(LIBS) 

%: %.cpp
	%(CXX) $(CXXFLAGS) $^ -o %@ $(LIBS)

#Para evitar 'actualizar' un ejecutable llamado clean
.PHONY: clean
clean:
	rm -f $(APPNAME) *.o

.PHONY: gitignore
gitignore: 
	echo $(APPNAME) > .gitignore
