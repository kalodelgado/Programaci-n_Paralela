#!/usr/bin/python3

import random
import math

random.seed()
num_tosses = int(input("Cantidad de intentos: "))
pi_hits = 0
for y in range(num_tosses):
    x = random.randint(0,10000)/10000
    y = random.randint(0,10000)/10000
    if  math.sqrt(math.pow(x,2) + math.pow(y,2)) < 1 :
        pi_hits = pi_hits+1
estimado_pi = 4*pi_hits/num_tosses
print("Estimacion de PI: ")
print(estimado_pi)
print("Desviacion:")
print(estimado_pi - 3.14159265358979323846)
    