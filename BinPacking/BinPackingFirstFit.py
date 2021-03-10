from time import time

def pedir_datos():
	capacidad = input('Digite la capacidad de los bin packets: ')
	arreglo_datos = input('Digite la lista de pesos separados por comas (v1,v2,v3...): ')
	return capacidad, arreglo_datos
		
def acomodar_datos(data, packets, capacidad):
	for i in range (len(data)):
		entro = False
		
		for j in range (len(packets)):
			if int(data[i]) <= int(packets[j]):
				packets[j] = int(packets[j]) - int(data[i])
				print("El numero: " + str(data[i]) + " entro en el bin packet: " + str(j + 1))
				entro = True
				break
				
		if entro == False:
			packets.append(capacidad)
			packets[len(packets)-1] = int(packets[len(packets)-1]) - int(data[i])
			print("El numero: " + str(data[i]) + " entro en el bin packet: " + str(len(packets)))
	
	return packets

def main ():
	capacidad = 0
	datos = []
	bin_packets = []
	
	capacidad, datos = pedir_datos()
	
	start_time = time()

	datos = datos.split(',')
	bin_packets.append(capacidad);
	
	datos.sort(reverse=True)
	
	bin_packets = acomodar_datos(datos, bin_packets, capacidad)
	
	elapsed_time = time() - start_time
	
	print("Quedaron esta cantidad de bin packets: " + str(len(bin_packets)))
	print(bin_packets)
	print("Tiempo: " + str(elapsed_time))

main ()
