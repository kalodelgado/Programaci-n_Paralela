
public class Liviano extends Vehiculo
{
    // instance variables
    private int puertas;
    private String peso;
    private Estilo estilo;
    //Constructor de la clase
    public Liviano(String tipoVehiculo, Estilo estilo, String marca, 
                   String modelo, int anno, String vin,String cilindrada, 
                   String placa, int precio, Combustible combustible, 
                   Transmision transmision,int puertas,String peso,
                   Propietario propietario){
        super(tipoVehiculo,marca,modelo,anno,vin,cilindrada,placa,precio,
              combustible,transmision,propietario);
        this.puertas = puertas;
        this.peso = peso;
        this.estilo = estilo;
    }
    //gets de la clase
    public Estilo getEstilo(){
        return estilo;
    }
    
    public int getPuertas(){
        return puertas;    
    }
    
    public String getPeso(){
        return peso;    
    }
}
