package calc;


import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.Enumeration;

public class BroadcastThread extends Thread {
	private String groupIP;
	private int port;
	int BROADCASTTIME = 5;
	
	public BroadcastThread(String groupIP, int port) {
		this.groupIP = groupIP;
		this.port = port;
	}
	
	public void run() {
		
		try {
			String localIP;
			localIP = Inet4Address.getLocalHost().getHostAddress();
		
		
			Enumeration<NetworkInterface> n = NetworkInterface.getNetworkInterfaces();
		    for (; n.hasMoreElements();) {
		        NetworkInterface e = n.nextElement();
	
		        Enumeration<InetAddress> a = e.getInetAddresses();
		        if (e.getName().equals("eth0")) {
			        for (; a.hasMoreElements();) {
			        	InetAddress addr = a.nextElement();
			            if (addr instanceof Inet4Address)
			            	localIP = addr.getHostAddress();
			        }
		        }
		    }
			
			InetAddress group = InetAddress.getByName(this.groupIP);
	        DatagramPacket packet;
	        DatagramSocket udpsocket = new DatagramSocket();
	        packet = new DatagramPacket(localIP.getBytes(), localIP.getBytes().length, group, this.port);
	        udpsocket.setBroadcast(true);
	        int i = 0;
	        while (i < BROADCASTTIME) {
	        	udpsocket.send(packet);
				sleep(500);
				i++;
	        }
	        udpsocket.close();
	        
		}
		catch (UnknownHostException e) {
			e.printStackTrace();
		} 
		catch (SocketException e) {
			e.printStackTrace();
		} 
		catch (IOException e) {
			e.printStackTrace();
		}
		catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
}
