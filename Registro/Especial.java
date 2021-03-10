
public class Especial extends Vehiculo
{
    // instance variables    
    private boolean orugas;
    private boolean articulado;
    
    public Especial(String tipoVehiculo,String marca, String modelo, 
                    int anno, String vin,String cilindrada, String placa, 
                    int precio, Combustible combustible, Transmision transmision,
                    boolean orugas,boolean articulado,Propietario propietario){
        super(tipoVehiculo,marca,modelo,anno,vin,cilindrada,placa,precio,
              combustible,transmision,propietario);
        this.orugas = orugas;
        this.articulado = articulado;
    }
    
    public boolean getOrugas(){
        return orugas;    
    }
    
    public boolean getArticulado(){
        return articulado;    
    }    
}
