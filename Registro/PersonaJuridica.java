public class PersonaJuridica extends Propietario
{
    // instance variables
    private String nombre;
    private Tipo tipoJ;//tipo de empresa
    
    public PersonaJuridica(String tipo, String cedula, String nombre, 
                           Tipo tipoEmpresa){
        super(tipo,cedula);
        this.nombre = nombre;
        this.tipoJ = tipoEmpresa;
    }
    
    public String getNombre(){
        return nombre;    
    }
    //distinto nombre para diferenciar el de la clase padre
    public Tipo getTipoJuridico(){
        return tipoJ;
    }
}
