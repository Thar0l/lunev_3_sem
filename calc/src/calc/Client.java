package calc;

import java.io.*;
import java.net.*;

public class Client {
	public static void main(String[] args) throws IOException, InterruptedException, ClassNotFoundException {
		MulticastSocket multicastsocket = new MulticastSocket(4446);
		String groupIP = "224.224.224.224";
		InetAddress group = InetAddress.getByName(groupIP);
		multicastsocket.joinGroup(group);

		DatagramPacket packet;
		byte[] buf = new byte[256];
		packet = new DatagramPacket(buf, buf.length);
		String receivedaddress = "0";
		Boolean connected = false;
		
		System.out.println("Waiting for broadcast...");
	    while (!connected) {
	    	multicastsocket.receive(packet);
	    	receivedaddress = new String(packet.getData());
		    System.out.println("Broadcast received from group : " + groupIP+ "; ServerIP =  " + receivedaddress);
		    connected = true;
	    }
	    multicastsocket.leaveGroup(group);
	    multicastsocket.close();
	    Thread.sleep(10000);
	    
	    System.out.println("Connecting to server...");
	    InetAddress serverAddress = InetAddress.getByName(receivedaddress);
	    Socket tcpsocket = new Socket(serverAddress, 1234);
	    System.out.println("Connected.");
	    
	    ObjectInputStream inputStream   = new ObjectInputStream(tcpsocket.getInputStream());
	    ObjectOutputStream outputStream = new ObjectOutputStream(tcpsocket.getOutputStream());
	    Thread.sleep(2000);
	    System.out.println("Sending data ...");
	    Message testmsg = new Message("test");
	    
	    outputStream.writeObject(testmsg);
	    testmsg = (Message) inputStream.readObject();
	    System.out.println("Data: "+testmsg.getText());

	    tcpsocket.close();
	}
}
