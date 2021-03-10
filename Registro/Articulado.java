public class Articulado extends Vehiculo
{
    // instance variables
    private double peso;
    private int ejes;
    private int articulaciones;
    
    public Articulado(String tipoVehiculo,String marca, String modelo, int anno, 
                      String vin, String cilindrada, String placa, int precio, 
                      Combustible combustible, Transmision transmision,
                      double peso,int ejes, int articulaciones,
                      Propietario propietario){
        super(tipoVehiculo,marca,modelo,anno,vin,cilindrada,placa,precio,
              combustible,transmision,propietario);
        this.peso = peso;
        this.ejes = ejes;
        this.articulaciones = articulaciones;
    }
    
    public double getPeso(){
        return peso;    
    }
    
    public int getEjes(){
        return ejes;    
    }
    
    public int getArticulaciones(){
        return articulaciones;    
    }
}
