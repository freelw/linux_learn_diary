import java.net.InetAddress;
import java.util.concurrent.TimeUnit;
public class JavaInetAddressGetAllByNameExample1 {  
    public static void main (String[] args) {
        try {  
            while (true) {
                InetAddress[] Addresses =  InetAddress.getAllByName("f11.maxxipoint.com");  
                System.out.println("123");
                for (InetAddress address : Addresses)  
                    System.out.println(address.getHostAddress());
                TimeUnit.MILLISECONDS.sleep(1000);
            }
        }  
        catch (Exception e) {  
            System.out.println("Could not find f11.maxxipoint.com");  
        }  
    }  
}