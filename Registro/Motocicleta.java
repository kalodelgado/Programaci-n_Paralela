public class Motocicleta extends Vehiculo
{
    // instance variables
    private int numeroRuedas;
    
    public Motocicleta(String tipoVehiculo, String marca, String modelo,
                       int anno, String vin, String cilindrada, String placa, 
                       int precio,Combustible combustible, Transmision transmision,
                       int numRuedas,Propietario propietario){
        super(tipoVehiculo, marca,modelo,anno,vin,cilindrada,placa,precio,
              combustible,transmision,propietario);
              //"Llama" al constructor de la clase padre
        this.numeroRuedas = numRuedas;
    }
    
    public int getNumeroRuedas(){
        return numeroRuedas;    
    }
}