public class Nodo
{
    // instance variables
    private Vehiculo vehiculo;
    public Nodo hijoMenor = null;
    public Nodo hijoMayor = null;
    
    /**
     * Constructors for objects of class Nodo
     */
    public Nodo(){}
    
    public Nodo(Vehiculo vehiculo)
    {
        this.vehiculo = vehiculo;
        hijoMenor = null;
        hijoMayor = null;
    }
    //Metodos set and get
    public void setVehiculo(Vehiculo vehiculo){
        this.vehiculo = vehiculo;
    }
    
    public void setHijoMenor(Nodo hijo){
        this.hijoMenor = hijo;
    }
    
    public void setHijoMayor(Nodo hijo){
        this.hijoMayor = hijo;
    }
    
    public Vehiculo getVehiculo(){
        return this.vehiculo;
    }
    
    public String getVin(){
        return this.vehiculo.getVin();
    }
    
    public Nodo getHijoMayor(){
        return this.hijoMayor;
    }
    
    public Nodo getHijoMenor(){
        return this.hijoMenor;
    }
    //Metodo que pasa los datos del nodo a string para imprimirlos mejor
    public String toString(){
        String datos = "Tipo: "+vehiculo.getTipo()+"\nMarca: "+
        vehiculo.getMarca()+"\nModelo: "+vehiculo.getModelo()+"\nAño: "
        +vehiculo.getAnno()+"\nCilindrada: "+vehiculo.getCilindrada()+
        "\nPlaca: "+vehiculo.getPlaca()+"\nPrecio: "+vehiculo.getPrecio()+
        "\nCombustible: "+vehiculo.getCombustible()+"\nTransmision: "+
        vehiculo.getTransmision();
        if(vehiculo instanceof Motocicleta){
            Motocicleta moto = (Motocicleta)vehiculo;
            datos += "\nNumero de Ruedas: "+moto.getNumeroRuedas();
        }else if(vehiculo instanceof Bus){
            Bus bus = (Bus)vehiculo;
            datos += "\nNumero de Asientos: "+bus.getNumeroAsientos();
        }else if(vehiculo instanceof Especial){
            Especial especial = (Especial)vehiculo;
            datos += "\nOrugas: "+especial.getOrugas()
                     +"\nArticulado: "+especial.getArticulado();
        }else if(vehiculo instanceof Pesado){
            Pesado pesado = (Pesado)vehiculo;
            datos += "\nPeso: "+pesado.getPeso()
                     +"\nEjes: "+pesado.getEjes();
        }else if(vehiculo instanceof Articulado){
            Articulado articulado = (Articulado)vehiculo;
            datos += "\nPeso: "+articulado.getPeso()
                     +"\nEjes: "+articulado.getEjes()
                     +"\nArticulaciones: "+articulado.getArticulaciones();
        }else{
            Liviano liv = (Liviano)vehiculo;
            datos += "\nEstilo: "+liv.getEstilo()
                     +"\nPeso: "+liv.getPeso()
                     +"\nNumero Puertas: "+liv.getPuertas();
        }
        datos += "\nCaracter de Propietario: "+
                 vehiculo.getPropietario().getTipo()
                 +"\nCedula: "+vehiculo.getPropietario().getCedula();
        if(vehiculo.getPropietario() instanceof PersonaJuridica){
            PersonaJuridica duenno=(PersonaJuridica)vehiculo.getPropietario();
            datos += "\nNombre: "+duenno.getNombre()
                     +"\nTipo Empresa: "+duenno.getTipoJuridico();
        }else{
            PersonaFisica pers =(PersonaFisica)vehiculo.getPropietario();
            datos += "\nNombre Completo: "+pers.getNombre()
                     +" "+pers.getPrimerApellido()+
                     " "+pers.getSegundoApellido();
        }
        
        return datos;
    }
}