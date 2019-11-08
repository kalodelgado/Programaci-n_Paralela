import json
import random
import pprint

# Returns a json formatted string for any custom class instance, dict of instances or list of instances.
# From: https://stackoverflow.com/questions/2343535/easiest-way-to-serialize-a-simple-class-object-with-simplejson
def json_repr(obj):
    """Represent instance of a class as JSON.
    Arguments:
    obj -- any object
    Return:
    String that reprent JSON-encoded object.
    """
    def serialize(obj):
        """Recursively walk object's hierarchy."""
        if isinstance(obj, (bool, int, float, str)):
            return obj
        elif isinstance(obj, dict):
            obj = obj.copy()
            for key in obj:
                obj[key] = serialize(obj[key])
            return obj
        elif isinstance(obj, list):
            return [serialize(item) for item in obj]
        elif isinstance(obj, tuple):
            return tuple(serialize([item for item in obj]))
        elif hasattr(obj, '__dict__'):
            return serialize(obj.__dict__)
        else:
            return repr(obj) # Don't know how to handle, convert to string
    return json.dumps(serialize(obj), indent=4, separators=(',', ': ')) # agrega sangrado y separadores entre objetos y atributos y valores

class Vertice:
    def __init__(self, idvrt: int):
        self.idvrt = idvrt  # id del vertice
        self.adys = []  # lista de adyacencias de vrt

random.seed() 
num_vrts = int(input("Cantidad de vertices: "))
p = float(input("Probabilidad de adyacencia: "))
vertices = {}               # se crea el map de vertices vacio
vertices['vertices'] = []   # se agrega un arreglo como unica entrada al map que contendra todos los datos

# se agregan todos los vertices vacios
for i in range(num_vrts):
    vrt = Vertice(i)    # se crea un vertice nuevo para llenar vertices con n elementos
    vertices['vertices'].append(vrt)

# se llenan las listas de adyacencia de cada vertice
for i in range(num_vrts):
    for j in range(i+1,num_vrts):
        if random.randint(0,100000)/100000 < p:
            vertices['vertices'][i].adys.append(j)
            vertices['vertices'][j].adys.append(i)    
with open('grafo_20.json', 'w') as outfile:
    outfile.write(json_repr(vertices))

json_data = open('grafo_20.json').read() 
data = json.loads(json_data)
pp = pprint.PrettyPrinter(indent=4)
pp.pprint(data)