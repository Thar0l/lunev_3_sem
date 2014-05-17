package calc;


import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;


public class BroadcastThread extends Thread {
	private String groupIP;
	private int port;
	
	public BroadcastThread(String groupIP, int port) {
		this.groupIP = groupIP;
		this.port = port;
	}
	
	public void run() {
		
		try {
			String data = "SRVENABLED";
			
			InetAddress group = InetAddress.getByName(this.groupIP);
	        DatagramPacket packet;
	        DatagramSocket udpsocket = new DatagramSocket();
	        packet = new DatagramPacket(data.getBytes(), data.getBytes().length, group, this.port);
	        udpsocket.setBroadcast(true);
	        udpsocket.send(packet);
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
	}
}
