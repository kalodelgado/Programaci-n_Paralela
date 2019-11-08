#!/usr/bin/python3
"""
Parsea la linea de comandos sacando los valores de los parametros del programa
Se puede usar asi:
reading_argv -h --> reading_argv.py -i <inputfile> -o <outputfile> # es como la ayuda de la operacion
reading_argv -i nombre_archivo_entrada.txt -o nombre_archivo_salida.txt
reading_argv --ifile nombre_archivo_entrada.txt --ofile nombre_archivo_salida.txt

Fuente: https://www.tutorialspoint.com/python3/python_command_line_arguments.htm
"""
import sys, getopt

def main(argv):
   inputfile = ''
   outputfile = ''
   try:
      ## parsea la lista de parametros de consola
	  ## en opts queda una lista de pares (opcion, valor)
	  ## en args queda una lista de valores sin las opciones
      opts, args = getopt.getopt(argv,"h:i:o:",["ifile=","ofile="])
   except getopt.GetoptError:
      print ('reading_argv.py -i <inputfile> -o <outputfile>')
      sys.exit(2)
   for opt, arg in opts:
      if opt == '-h':
         print ('reading.py -i <inputfile> -o <outputfile>')
         sys.exit()
      elif opt in ("-i", "--ifile"):
         inputfile = arg
      elif opt in ("-o", "--ofile"):
         outputfile = arg
   print ('Input file is "', inputfile)
   print ('Output file is "', outputfile)

if __name__ == "__main__": 	## implica que reading_argv está siendo ejecutado directamente y no por otro modulo
   main(sys.argv[1:])		## le pasa a main la lista de opciones, parametros obtenida del sistema