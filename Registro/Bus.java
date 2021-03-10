
public class Bus extends Vehiculo
{
    // instance variables
    private int numeroAsientos;
    
    public Bus(String tipoVehiculo,String marca, String modelo, int anno, 
               String vin,String cilindrada, String placa, int precio, 
               Combustible combustible, Transmision transmision,
               int numAsientos,Propietario propietario){
        super(tipoVehiculo,marca,modelo,anno,vin,cilindrada,placa,precio,
              combustible,transmision,propietario);
              //"Llama" al constructor de la clase padre
        this.numeroAsientos = numAsientos;
    }
    
    public int getNumeroAsientos(){
        return numeroAsientos;    
    }
}
