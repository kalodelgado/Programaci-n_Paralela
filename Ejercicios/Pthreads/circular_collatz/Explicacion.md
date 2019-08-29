El codigo inicial no da la solucion porque el while saca al primer thread que de con el resultado 1, al pasar esto la barrera se queda esperando.
Posee el problema de que no se accede a todos los campos del vector numbers y ademas se incluye pthread_barrier (barrier ya lo incluye pthread)
Ademas no libera memoria y usa varibles estaticas, practica que el profesor dijo no es la mas correcta
