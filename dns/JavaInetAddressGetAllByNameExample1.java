import java.net.InetAddress;  
public class JavaInetAddressGetAllByNameExample1 extends Thread {  
    public static void main (String[] args) {
        try {  
            while (true) {
                InetAddress[] Addresses =  InetAddress.getAllByName("f11.maxxipoint.com");  
                for (InetAddress address : Addresses)  
                    System.out.println(address.getHostAddress());
                Thread.sleep(1000);
            }
        }  
        catch (Exception e) {  
            System.out.println("Could not find f11.maxxipoint.com");  
        }  
    }  
}