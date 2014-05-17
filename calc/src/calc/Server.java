package calc;

import java.io.*;
import java.net.*;

import Jama.Matrix;

public class Server {
	public static void main(String[] args) {
		try {
		MulticastSocket multicastsocket = new MulticastSocket(4446);
		String groupIP = "224.224.224.224";
		InetAddress group = InetAddress.getByName(groupIP);
		multicastsocket.joinGroup(group);

		DatagramPacket packet;
		byte[] buf = new byte[256];
		packet = new DatagramPacket(buf, buf.length);
		Boolean connected = false;
		
		System.out.println("Waiting for clients...");
	    while (!connected) {
	    	multicastsocket.receive(packet);
		    System.out.println("Broadcast received from group : " + groupIP+ "; ClientIP =  " + packet.getAddress());
		    connected = true;
	    }
	    multicastsocket.leaveGroup(group);
	    multicastsocket.close();

	    
	    System.out.println("Connecting to client...");
	    InetAddress serverAddress = packet.getAddress();
	    Socket tcpsocket = new Socket(serverAddress, 1234);
	    System.out.println("Connected.");
	    
	    ObjectInputStream inputStream   = new ObjectInputStream(tcpsocket.getInputStream());
	    ObjectOutputStream outputStream = new ObjectOutputStream(tcpsocket.getOutputStream());
	    //Thread.sleep(2000);
	    System.out.println("Receiving data ...");
	    
	    Message testmsg = null;
	    testmsg = (Message) inputStream.readObject();
	    Matrix m = testmsg.getMatrix();
	    testmsg =new Message(m.det());
	    System.out.println("Sending data ...");
	    outputStream.writeObject(testmsg);

	    tcpsocket.close();
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		} catch (UnknownHostException e) {
			e.printStackTrace();
		} catch (IOException e) {
			if (e.getClass().getSimpleName().equals("EOFException")) {
				System.err.println("No connection to client.");
			} else if (e.getMessage().equals("Broken pipe")) {
				System.err.println("No connection to client.");
			} else {
				System.out.println("!!! "+e.getClass().getSimpleName());
			e.printStackTrace();
			}
		}
		
	}
}
