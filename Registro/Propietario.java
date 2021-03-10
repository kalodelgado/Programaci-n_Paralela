public class Propietario
{
    // instance variables
    private String tipo;//variable que guardará si es "c" o "p"
    //"p" PersonaFisica, "c" PersonaJuridica
    private String cedula;
    
    public Propietario(String tipo, String cedula){
        this.tipo = tipo;
        this.cedula = cedula;
    }
    
    public String getTipo(){
        return tipo;
    }
    
    public String getCedula(){
        return cedula;    
    }
}
