package calc;

import java.io.*;
import java.net.*;

import Jama.Matrix;

public class Server {
	public static void main(String[] args) {
		while (true)
		try {
					
			while(true)
			{
				MulticastSocket multicastsocket = new MulticastSocket(4444);
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
				    System.out.println("Multicast received from group : " + groupIP+ "; ClientIP =  " + packet.getAddress());
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
		
			    System.out.println("Receiving data ...");
			    
			    Message msg = null;
			    msg = (Message) inputStream.readObject();
			    Matrix matrix = msg.getMatrix();
			    int startindex = msg.getStartMinorIndex();
			    int endindex = msg.getEndMinorIndex();
			    int size = msg.getMatrixSize();
			    System.out.println("Calculating minors from "+ startindex+" to "+endindex);
			  
			    double result = 0.0;
		
			    /*********************************************/
			    
				for (int i = startindex; i <= endindex; i++)
				{
					int arr[] = new int[size-1];
					for (int k = 1; k < size; k++) 
						arr[k-1] = k;
					if (i > 0) arr[i-1] = 0;
					
					Matrix minor = matrix.getMatrix(0, size-2, arr);
					
					if (i%2 == 0)
						result += minor.det();
					else
						result -= minor.det();
				}
			    
			    /************************************************/
			    
			    System.out.println("Sending data ...");
			    
			    msg =new Message(result);
			    outputStream.writeObject(msg);
			   /* msg = (Message) inputStream.readObject();
			    
			    while ( ! msg.getText().equals("Exit")) {
			    	msg = (Message) inputStream.readObject();
			    }*/
			    tcpsocket.close();
			    
			}
			
	
		    
			} catch (ClassNotFoundException e) {
				e.printStackTrace();
			} catch (UnknownHostException e) {
				e.printStackTrace();
			} catch (IOException e) {
				if (e.getClass().getSimpleName().equals("EOFException")) {
					System.err.println("Connection to client lost.");
					
				} else if (e.getMessage().equals("Broken pipe")) {
					System.err.println("Connection to client lost.");
				} else {
				e.printStackTrace();
				}
			}
		}
	
}
