import javax.swing.JOptionPane;

public class Main{    
   public static void main(String[] args){
       Arbol garage = new Arbol();
       Object objeto;
       
       garage.leerArchivo();
       
       String msj = "Elija una opción \n";
       msj += "1. Buscar por VIN \n";
       msj += "2. Buscar por Placa\n";
       msj += "3. Buscar por Cedula\n";
       msj += "4. Eliminar por VIN\n";
       msj += "0. Salir";
        
       boolean seguir = true;        
       while(seguir){            
           String entrada = JOptionPane.showInputDialog(msj);
           
           if(entrada.compareTo("1") == 0)//Busqueda por vin(llave)
           {
               entrada = JOptionPane.showInputDialog("Digite el VIN");
               objeto = garage.buscarPorVin(entrada);
               if(!garage.esVacio()){
                   if(objeto != null){
                       JOptionPane.showMessageDialog(null,"Vehiculo encontrado\n"+objeto);
                   }else{
                       JOptionPane.showMessageDialog(null,"El vehiculo digitado no existe");
                   }
               }else{
                   JOptionPane.showMessageDialog(null,"\nEl Registro está vacio");
               }
           }else if(entrada.compareTo("2") == 0)//Busqueda por placa(profundidad)
           {
               entrada = JOptionPane.showInputDialog("Digite la Placa");
               objeto = garage.busquedaProfundidad(entrada);
               if(!garage.esVacio()){
                   if(objeto != null){
                       JOptionPane.showMessageDialog(null,objeto);
                   }else{
                       JOptionPane.showMessageDialog(null,"El vehiculo digitado no existe");
                   }
               }else{
                   JOptionPane.showMessageDialog(null,"\nEl Registro está vacio");
               }
           }else if(entrada.compareTo("3") == 0){
               seguir = false;
           }else if(entrada.compareTo("4") == 0){
               entrada = JOptionPane.showInputDialog("Digite el VIN");
               boolean obj = garage.eliminarNodo(entrada);
               if(!garage.esVacio()){
                   if(obj == true){
                       JOptionPane.showMessageDialog(null,"El vehiculo ha sido eliminado\n");
                   }else{
                       JOptionPane.showMessageDialog(null,"El vehiculo digitado no se ha encontrado");
                   }
               }else{
                   JOptionPane.showMessageDialog(null,"\nEl Registro está vacio");
               }
           }else if(entrada.compareTo("0") == 0){
               seguir = false;
           }else{
               JOptionPane.showMessageDialog(null, "Digite una opcion valida", "Error", JOptionPane.ERROR_MESSAGE);
           }
       }
   }
}