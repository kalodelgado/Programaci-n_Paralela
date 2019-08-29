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
	$(CC) $(FLAGS) $^ -o $@ -lm $(LIBS)

#Para evitar 'actualizar' un ejecutable llamado clean
.PHONY: clean
clean:
	rm -f $(APPNAME) *.o

.PHONY: gitignore
gitignore: 
	echo $(APPNAME) > .gitignore
