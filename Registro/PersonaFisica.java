
public class PersonaFisica extends Propietario
{
    // instance variables
    private String nombre;
    private String primerApellido;
    private String segundoApellido;
    
    public PersonaFisica(String tipo, String cedula,String nombre, 
                         String primerApellido, String segundoApellido){
        super(tipo,cedula);
        this.nombre = nombre;
        this.primerApellido = primerApellido;
        this.segundoApellido = segundoApellido;
    }
    
    public String getNombre(){
        return nombre;    
    }
    
    public String getPrimerApellido(){
        return primerApellido;    
    }
    
    public String getSegundoApellido(){
        return segundoApellido;    
    }
}
