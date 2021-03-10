public class Vehiculo
{
    // instance variables
    private String tipoVehiculo;
    private String marca;
    private String modelo;
    private int anno;
    private String vin;//VIN
    private String cilindrada;
    private String placa;
    private int precio;
    private Combustible combustible;
    private Transmision transmision;
    private Propietario propietario;
    
    public Vehiculo(String tipoVehiculo, String marca, String modelo, 
                    int anno, String vin,String cilindrada, String placa, 
                    int precio,Combustible combustible, Transmision transmision,
                    Propietario propietario){
        this.tipoVehiculo = tipoVehiculo;
        this.marca = marca;
        this.modelo = modelo;
        this.anno = anno;
        this.vin = vin;
        this.cilindrada = cilindrada;
        this.placa = placa;
        this.precio = precio;
        this.combustible = combustible;
        this.transmision = transmision;
        this.propietario = propietario;
    }
    //gets de la clase
    public Propietario getPropietario(){
        return propietario;
    }
    
    public int getPrecio(){
        return precio;    
    }
    
    public String getPlaca(){
        return placa;    
    }
    
    public String getMarca(){
        return marca;    
    }
    
    public String getModelo(){
        return modelo;    
    }
        
    public int getAnno(){
        return anno;    
    }
    
    public String getVin(){
        return vin;    
    }
    
    public String getCilindrada(){
        return cilindrada;    
    }
    
    public Combustible getCombustible(){
        return combustible;    
    }
    
    public Transmision getTransmision(){
        return transmision;    
    }
    
    public String getTipo(){
        return tipoVehiculo;    
    }
}