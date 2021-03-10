import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class Arbol
{
    // instance variables
    private Nodo raiz;
    private List<Vehiculo> listaProfundidad = new ArrayList<Vehiculo>();

    /**
     * Constructor for objects of class Arbol
     */
    public Arbol(){  
    }
    //get Nodo raiz
    public Nodo getRaiz(){
        return this.raiz;
    }    
    //metodo que se utilizará para hacer "validaciones"
    public boolean esVacio(){
        if(this.raiz == null) 
            return true;
        else 
            return false;
    }
        
    public void leerArchivo(){
        FileReader fileReader = null;
        BufferedReader buffReader = null;
        
        //En esta lista se irán guardando todos
        //los vehiculos que se crearán más adelante
        List<Vehiculo> vehiculo = new ArrayList<Vehiculo>();
        
        //Todo lo necesario para abrir y leer un archivo
        try {
            fileReader = new FileReader("C:\\GIT\\automoviles.txt");
            buffReader = new BufferedReader(fileReader);
            //String que servirá para luego guardar sus partes en un arreglo
            String linea;
            while((linea = buffReader.readLine()) !=  null) {
                String[] partes = linea.split(",");                
                //Comparacion para saber que tipo de vehiculo es
                if(partes[0].compareTo("Motocicleta") == 0){
                    Motocicleta moto;
                    if(partes[11].compareTo("p") == 0){
                        PersonaFisica persona = new PersonaFisica(partes[11],
                                                    partes[12],partes[13],
                                                    partes[14],partes[15]);
                        //Cada parte del arreglo se usa para crear una instanci              
                        moto=new Motocicleta(partes[0],partes[1],partes[2],
                             Integer.parseInt(partes[3]),partes[4],
                             partes[5],partes[6],Integer.parseInt(partes[7]),
                             Combustible.valueOf(partes[8]),
                             Transmision.valueOf(partes[9]),
                             Integer.parseInt(partes[10]),persona);
                    }else{
                        PersonaJuridica persona=new PersonaJuridica(
                                                partes[11],partes[12],
                                                partes[13],Tipo.valueOf(partes[14]));
                        
                        moto=new Motocicleta(partes[0],partes[1],partes[2],
                             Integer.parseInt(partes[3]),partes[4],
                             partes[5],partes[6],Integer.parseInt(partes[7]),
                             Combustible.valueOf(partes[8]),
                             Transmision.valueOf(partes[9]),
                             Integer.parseInt(partes[10]),persona);
                    }
                    //agrega el nuevo vehiculo a la lista
                    vehiculo.add(moto);
                }else if(partes[0].compareTo("Pesado") == 0){
                    Pesado pesado;
                    if(partes[11].compareTo("p") == 0){
                        PersonaFisica persona=new PersonaFisica(partes[12],
                                                  partes[13],partes[14],
                                                  partes[15],partes[16]);
                        
                        pesado = new Pesado(partes[0],partes[1],partes[2],
                                     Integer.parseInt(partes[3]),partes[4],
                                     partes[5],partes[6],
                                     Integer.parseInt(partes[7]),
                                     Combustible.valueOf(partes[8]),
                             Transmision.valueOf(partes[9]),
                                     Double.parseDouble(partes[10]),
                                     Integer.parseInt(partes[11]),persona);
                    }else{
                        PersonaJuridica persona=new PersonaJuridica(
                                                partes[12],partes[13],
                                                partes[14],Tipo.valueOf(partes[15]));
                        
                        pesado = new Pesado(partes[0],partes[1],partes[2],
                                     Integer.parseInt(partes[3]),partes[4],
                                     partes[5],partes[6],
                                     Integer.parseInt(partes[7]),
                                     Combustible.valueOf(partes[8]),
                                     Transmision.valueOf(partes[9]),
                                     Double.parseDouble(partes[10]),
                                     Integer.parseInt(partes[11]),persona);
                    }                   
                    vehiculo.add(pesado);
                }else if(partes[0].compareTo("Articulado") == 0){
                    Articulado articulado;
                    if(partes[11].compareTo("p") == 0){
                        PersonaFisica persona=new PersonaFisica(partes[12],
                                                  partes[13],partes[14],
                                                  partes[15],partes[16]);
                        
                        articulado=new Articulado(partes[0],partes[1],
                                       partes[2],Integer.parseInt(partes[3]),
                                       partes[4],partes[5],partes[6],
                                       Integer.parseInt(partes[7]),
                                       Combustible.valueOf(partes[8]),
                                       Transmision.valueOf(partes[9]),
                                       Double.parseDouble(partes[10]),
                                       Integer.parseInt(partes[11]),2,persona);
                    }else{
                        PersonaJuridica persona=new PersonaJuridica(partes[12],
                                                partes[13],partes[14],
                                                Tipo.valueOf(partes[15]));
                        
                        articulado=new Articulado(partes[0],partes[1],partes[2],
                                       Integer.parseInt(partes[3]),partes[4],
                                       partes[5],partes[6],
                                       Integer.parseInt(partes[7]),
                                       Combustible.valueOf(partes[8]),
                                       Transmision.valueOf(partes[9]),
                                       Double.parseDouble(partes[10]),
                                       Integer.parseInt(partes[11]),2,persona);
                    }                   
                    vehiculo.add(articulado);
                }else if(partes[0].compareTo("Autobus") == 0){
                    Bus bus;//creacion de instancia de la clase Bus
                    if(partes[11].compareTo("p") == 0){
                        PersonaFisica persona=new PersonaFisica(partes[11],
                                                  partes[12],partes[13],
                                                  partes[14],partes[15]);
                        
                        bus=new Bus(partes[0], partes[1],partes[2],
                            Integer.parseInt(partes[3]),partes[4],
                            partes[5],partes[6],Integer.parseInt(partes[7]),
                            Combustible.valueOf(partes[8]),
                            Transmision.valueOf(partes[9]),
                            Integer.parseInt(partes[10]),persona);
                    }else{
                        PersonaJuridica persona=new PersonaJuridica(
                                                partes[11],partes[12],
                                                partes[13],Tipo.valueOf(partes[14]));
                        
                        bus=new Bus(partes[0], partes[1],partes[2],
                            Integer.parseInt(partes[3]),partes[4],
                            partes[5],partes[6],Integer.parseInt(partes[7]),
                            Combustible.valueOf(partes[8]),
                            Transmision.valueOf(partes[9]),
                            Integer.parseInt(partes[10]),persona);
                    }                   
                    vehiculo.add(bus);
                }else if(partes[0].compareTo("Especial") == 0){
                    Especial especial;
                    //se declaran estas variables para posteriormente  
                    //enviarlas a la clase Especial
                    boolean oruga;
                    boolean articulado;
                    if (partes[10].compareTo("t") == 0)
                        oruga = true;
                        //Se omiten los corchetes porque solo es una linea
                    else
                        oruga = false;
                    
                    if (partes[11].compareTo("t") == 0)
                        articulado = true;
                    else
                        articulado = false;
                    
                    if(partes[12].compareTo("p") == 0){
                        PersonaFisica persona =new PersonaFisica(partes[12],
                                                   partes[13],partes[14],
                                                   partes[15],partes[16]);
                        
                        especial=new Especial(partes[0],partes[1],partes[2],
                                     Integer.parseInt(partes[3]),partes[4],
                                     partes[5],partes[6],
                                     Integer.parseInt(partes[7]),
                                     Combustible.valueOf(partes[8]),
                                     Transmision.valueOf(partes[9]),
                                     oruga,articulado,persona);
                    }else{
                        PersonaJuridica persona=new PersonaJuridica(
                                                partes[12],partes[13],
                                                partes[14],Tipo.valueOf(partes[15]));
                        if(partes[9].compareTo("N/A")!=0){
                            especial = new Especial(partes[0],partes[1],partes[2],
                                       Integer.parseInt(partes[3]),partes[4],
                                       partes[5],partes[6],
                                       Integer.parseInt(partes[7]),
                                       Combustible.valueOf(partes[8]),
                                       Transmision.valueOf(partes[9]),
                                       oruga,articulado,persona);
                        }else{
                            especial = new Especial(partes[0],partes[1],partes[2],
                                       Integer.parseInt(partes[3]),partes[4],
                                       partes[5],partes[6],
                                       Integer.parseInt(partes[7]),
                                       Combustible.valueOf(partes[8]),
                                       Transmision.NoAplicable,
                                       oruga,articulado,persona);
                        }
                    }                   
                    vehiculo.add(especial);
                }else{//Es liviano
                    //El Estilo irá en el campo tipoVehiculo de la clase Vehicul
                    Liviano carro;
                    if(partes[12].compareTo("p") == 0){
                        PersonaFisica persona=new PersonaFisica(partes[12],
                                                    partes[13],partes[14],
                                                    partes[15],partes[16]);
                        if(partes[8].compareTo("Gasolina")==0 || 
                           partes[8].compareTo("Diesel")==0)
                        {
                        //se hace esta distincion con el fin de no tildar 
                        //en la clase enum la palabra Hibrido 
                        carro = new Liviano("Liviano",
                                    Estilo.valueOf(partes[0]),
                                    partes[1],partes[2],
                                    Integer.parseInt(partes[3]),partes[4],
                                    partes[5],partes[6],
                                    Integer.parseInt(partes[7]),
                                    Combustible.valueOf(partes[8]),
                                    Transmision.valueOf(partes[9]),
                                    Integer.parseInt(partes[10]),
                                    partes[11],persona);
                        }else{
                            carro = new Liviano("Liviano",
                                    Estilo.valueOf(partes[0]),
                                    partes[1],partes[2],
                                    Integer.parseInt(partes[3]),partes[4],
                                    partes[5],partes[6],
                                    Integer.parseInt(partes[7]),
                                    Combustible.Hibrido,
                                    Transmision.valueOf(partes[9]),
                                    Integer.parseInt(partes[10]),
                                    partes[11],persona);
                        }
                    }else{
                        PersonaJuridica persona=new PersonaJuridica(
                                                partes[12],partes[13],
                                                partes[14],Tipo.valueOf(partes[15]));
                        
                        carro = new Liviano("Liviano",Estilo.valueOf(partes[0]),
                                    partes[1],partes[2],
                                    Integer.parseInt(partes[3]),partes[4],
                                    partes[5],partes[6],
                                    Integer.parseInt(partes[7]),
                                    Combustible.valueOf(partes[8]),
                                    Transmision.valueOf(partes[9]),
                                    Integer.parseInt(partes[10]),
                                    partes[11],persona);
                    }                   
                    vehiculo.add(carro);
                }
            }       
        }catch (FileNotFoundException e) {
            System.err.println("Error abriendo el archivo");
            e.printStackTrace(System.err);
        }catch (IOException e) {
            System.err.println("Error leyendo del archivo");
            e.printStackTrace(System.err);
        }finally {
            try {
                fileReader.close();
            } catch (IOException e) {
                System.err.println("Error cerrando el archivo");
                e.printStackTrace(System.err);      
            }
            try {
               buffReader.close();
            } catch (IOException e) {
                System.err.println("Error cerrando el archivo");
                e.printStackTrace(System.err);  
            }           
        }
        //Llama al metodo de agregar, para crear los nodos y el arbol
        for(int i=0; i < vehiculo.size(); i++){
            this.agregar(vehiculo.get(i));
        }        
    }
    
    public void agregar(Vehiculo vehiculo){
        //Se crea un objeto nodo y se le envia un vehiculo a su constructor
        Nodo nuevoNodo = new Nodo(vehiculo);
        //Llama al metodo de agregar nodo al arbol
        this.agregarNodo(nuevoNodo);
    }
    //Agrega un nodo al arbol
    public void agregarNodo(Nodo nodo){
        if(this.esVacio()){
            this.raiz = nodo;
        }
        else{
            Nodo puntero = this.raiz;
            boolean agregado = false;
            while(!agregado){
                if(nodo.getVin().compareTo(puntero.getVin()) > 0){
                    if(puntero.getHijoMayor() == null){
                        puntero.setHijoMayor(nodo);
                        agregado = true;
                    }else{
                        puntero = puntero.getHijoMayor();
                    }
                }else{
                    if(puntero.getHijoMenor() == null){
                        puntero.setHijoMenor(nodo);
                        agregado = true;
                    }else{
                        puntero = puntero.getHijoMenor();
                    } 
                }
            }
        }
    }    
    //Busqueda de vehiculo por VIN(llave)
    public Nodo buscarPorVin(String vin){
        Nodo temporal = this.raiz;
        while(temporal.getVin().compareTo(vin.toUpperCase()) != 0){
            if(vin.compareTo(temporal.getVin()) < 0) {
                temporal = temporal.getHijoMenor();
            }else{
                temporal = temporal.getHijoMayor();
            }
            //En caso de que no haya encontrado nada
            if(temporal == null){
                return null;
            }
        }        
        return temporal;
    }
    //Creacion lista de profundidad
    public void crearListaProfundidad(Nodo nodo)//Se recibiria la raiz
    {
        if(nodo == null)//"finaliza" el metodo
            return;//Al ser solo una linea se pueden omitir los corchetes
        
        listaProfundidad.add(nodo.getVehiculo());//agrega a la lista el vehicul
        crearListaProfundidad(nodo.getHijoMenor());//recorre subarbol izquie
        crearListaProfundidad(nodo.getHijoMayor());//recorre subarbol derech
    }
    
    public String busquedaProfundidad(String placa){
        if(listaProfundidad.isEmpty()){
            crearListaProfundidad(this.raiz);
        }
        String datos = "Vehiculo no existente";
        for(int i = 0; i < this.listaProfundidad.size(); i++){
            if(this.listaProfundidad.get(i).getPlaca().compareTo(placa.toUpperCase())==0){
                datos = "Vehiculo encontrado"+
                "\nTipo: "+listaProfundidad.get(i).getTipo()+
                "\nMarca: "+listaProfundidad.get(i).getMarca()
                +"\nModelo: "+listaProfundidad.get(i).getModelo()+
                "\nAño: "+listaProfundidad.get(i).getAnno()+
                "\nCilindrada: "+listaProfundidad.get(i).getCilindrada()+
                "\nPlaca: "+listaProfundidad.get(i).getPlaca()+
                "\nPrecio: "+listaProfundidad.get(i).getPrecio()+
                "\nCombustible: "+listaProfundidad.get(i).getCombustible()+
                "\nTransmision: "+listaProfundidad.get(i).getTransmision();
                
                if(listaProfundidad.get(i) instanceof Motocicleta){
                    Motocicleta moto = (Motocicleta)listaProfundidad.get(i);
                    datos += "\nNumero de Ruedas: "+moto.getNumeroRuedas();
                }else if(listaProfundidad.get(i) instanceof Bus){
                    Bus bus = (Bus)listaProfundidad.get(i);
                    datos += "\nNumero de Asientos: "+bus.getNumeroAsientos();
                }else if(listaProfundidad.get(i) instanceof Especial){
                    Especial especial = (Especial)listaProfundidad.get(i);
                    datos += "\nOrugas: "+especial.getOrugas()
                             +"\nArticulado: "+especial.getArticulado();
                }else if(listaProfundidad.get(i) instanceof Pesado){
                    Pesado pesado = (Pesado)listaProfundidad.get(i);
                    datos += "\nPeso: "+pesado.getPeso()
                             +"\nEjes: "+pesado.getEjes();
                }else if(listaProfundidad.get(i) instanceof Articulado){
                    Articulado articulado=(Articulado)listaProfundidad.get(i);
                    datos += "\nPeso: "+articulado.getPeso()
                             +"\nEjes: "+articulado.getEjes()
                             +"\nArticulaciones: "+
                             articulado.getArticulaciones();
                }else{
                    Liviano liv = (Liviano)listaProfundidad.get(i);
                    datos += "\nEstilo: "+liv.getEstilo()
                             +"\nPeso: "+liv.getPeso()
                             +"\nNumero Puertas: "+liv.getPuertas();
                }
                datos += "\nCaracter de Propietario: "+
                         listaProfundidad.get(i).getPropietario().getTipo()
                         +"\nCedula: "+
                         listaProfundidad.get(i).getPropietario().getCedula();
                if(listaProfundidad.get(i).getPropietario() instanceof PersonaJuridica)
                {
                    PersonaJuridica duenno;
                    duenno=(PersonaJuridica)listaProfundidad.get(i).getPropietario();
                    datos += "\nNombre: "+duenno.getNombre()
                             +"\nTipo Empresa: "+duenno.getTipoJuridico();
                }else{
                    PersonaFisica pers;
                    pers=(PersonaFisica)listaProfundidad.get(i).getPropietario();
                    datos += "\nNombre Completo: "+pers.getNombre()
                             +" "+pers.getPrimerApellido()+
                             " "+pers.getSegundoApellido();
                }                
                i = listaProfundidad.size();//hace terminar el ciclo
            }
        }
        return datos;
    }
    //Eliminar nodo
    public boolean eliminarNodo(String vin){
        Nodo auxiliar = this.raiz;
        Nodo padre = this.raiz;
        boolean esHijoIzq = true;
        while(auxiliar.getVin().compareTo(vin.toUpperCase()) != 0){
            padre = auxiliar;
            //padre siempre estará "arriba" del nodo a eliminar
            if(auxiliar.getVin().compareTo(vin.toUpperCase()) > 0){
                esHijoIzq=true;
                auxiliar = auxiliar.getHijoMenor();
            }else{
                esHijoIzq=false;
                auxiliar = auxiliar.getHijoMayor();
            }
            if(auxiliar == null){//No lo encontró
                return false;//porq el metodo retorna un boolean
            }
        }
        if(auxiliar.getHijoMenor()==null && auxiliar.getHijoMayor()==null){
            //quiere decir que es hoja
            if(auxiliar == this.raiz){
                this.raiz = null;
            }else if(esHijoIzq){
                padre.hijoMenor = null;
            }else{
                padre.hijoMayor = null;
            }
        }else if(auxiliar.getHijoMayor() == null){
            if(auxiliar == this.raiz){
                this.raiz = auxiliar.getHijoMenor();
            }else if(esHijoIzq){
                padre.hijoMenor = auxiliar.getHijoMenor();
            }else{
                padre.hijoMayor = auxiliar.getHijoMenor();
            }
        }else if(auxiliar.getHijoMenor() == null){
            if(auxiliar == this.raiz){
                this.raiz = auxiliar.getHijoMayor();
            }else if(esHijoIzq){
                padre.hijoMenor = auxiliar.getHijoMayor();
            }else{
                padre.hijoMayor = auxiliar.getHijoMenor();
            }
        }else{
            Nodo reemplazo = obtenerNodoReemplazo(auxiliar);
            if(auxiliar == this.raiz){
                this.raiz = reemplazo;
            }else if(esHijoIzq){
                padre.hijoMenor = reemplazo;
            }else{
                padre.hijoMayor = reemplazo;
            }
            reemplazo.hijoMenor = auxiliar.getHijoMenor();
        }
        listaProfundidad.clear();
        //La vacia porqeu si se buscó por placa primero el nodo seguiria existiendo
        return true;//si encontró el nodo a eliminar
    }
    //Metodo encargado de devolvernos el nodo reemplazo
    //El que reemplazará a quien vamos a eliminar
    public Nodo obtenerNodoReemplazo(Nodo aux){
        Nodo reemplazarPadre = aux;
        Nodo reemplazo = aux;
        Nodo auxiliar = aux.getHijoMayor();
        while(auxiliar != null)
        //recorrer para saber quien sera el nodo candidato a reemplazar 
        //a quien vamos a eliminar
        {
            reemplazarPadre = reemplazo;
            reemplazo = auxiliar;
            auxiliar = auxiliar.getHijoMenor();
        }
        if(reemplazo != aux.getHijoMayor()){
            reemplazarPadre.hijoMenor = reemplazo.getHijoMayor();
            reemplazo.hijoMayor = aux.getHijoMayor();
        }
        return reemplazo;
    }
}