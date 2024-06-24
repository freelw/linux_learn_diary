import java.io.*;
import java.net.InetAddress;
import java.util.concurrent.TimeUnit;
public class JavaInetAddressGetAllByNameExample1 {
    public static void run (String domain) throws Exception {
        try {
            InetAddress[] Addresses =  InetAddress.getAllByName(domain);  
            for (InetAddress address : Addresses)  
                System.out.println(address.getHostAddress());
        } 
        catch (Exception e) {
            e.printStackTrace(); 
            throw e;
        }
    }
    public static void openmany() {
        try {
            File f = new File("/tmp/a.txt");
            FileOutputStream fop = new FileOutputStream(f);
        }
        catch (Exception e) {
            
            System.out.println("open err");
        }
    }
    public static void main (String[] args) {
        /*
        fmapp-apollo.dev.familymart.com
        fmapp-apollo.sit.familymart.com  
        fmapp-apollo.uat.familymart.com
        fmapp-apollo.stage.familymart.com
        fmapp-apollo.pp
        rod.familymart.com
        f9.maxxipoint.com
        f11.maxxipoint.com
        api.maxxipoint.com
        app.maxxipoint.com
        risk-api.maxxipoint.com
        */
        for (int i = 0; i < 1000; ++ i) {
            openmany();
        }
        try {  
            while (true) {
                openmany();
                run("fmapp-apollo.dev.familymart.com");
                run("fmapp-apollo.sit.familymart.com");
                run("fmapp-apollo.uat.familymart.com");
                run("fmapp-apollo.stage.familymart.com");
                run("fmapp-apollo.prod.familymart.com");
                run("f9.maxxipoint.com");
                run("api.maxxipoint.com");
                run("app.maxxipoint.com");
                run("risk-api.maxxipoint.com");
                run("f11.maxxipoint.com");
                TimeUnit.MILLISECONDS.sleep(1000);
            }
        }  
        catch (Exception e) {  
            System.out.println("dns err");  
        }  
    }  
}