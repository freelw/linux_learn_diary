import java.io.*;
import java.net.InetAddress;
import java.util.concurrent.TimeUnit;

class MyThread extends Thread {
    static String domain;
    public void dns (String domain) throws Exception {
        try {
            InetAddress[] Addresses =  InetAddress.getAllByName(domain);  
            //for (InetAddress address : Addresses)  
                //System.out.println(address.getHostAddress());
        } 
        catch (Exception e) {
            e.printStackTrace(); 
            throw e;
        }
    }
    public InetAddress fun() throws Exception {
        InetAddress inet = InetAddress.getLocalHost();
        return inet;
        // System.out.println("域名：" + inet.getHostName()); // LP-BJ4556
        // System.out.println("IP地址：" + inet.getHostAddress()); // IP地址：2.0.0.137
    }

    @Override
    public void run() {

        try{
            /*InetAddress inet = fun();
            System.out.println("域名：" + inet.getHostName());
            System.out.println("IP地址：" + inet.getHostAddress());
            while (true) {
                fun();
            }*/
            while (true) {
                // dns("VM-0-13-centos");
                dns(domain);
            }
        }  
        catch (Exception e) {  
            System.out.println("err");  
        }  
    }
}

public class JavaGetLocalHostExample {
    
    
    public static void main (String[] args) {
        try{
            MyThread.domain = args[0];
            for (int i = 0; i < 50; ++ i) {
                Thread t1 = new MyThread();
                Thread t2 = new MyThread();
                t1.start();
                t2.start();
            }
        }  
        catch (Exception e) {  
            System.out.println("err");  
        }  
    }  
}