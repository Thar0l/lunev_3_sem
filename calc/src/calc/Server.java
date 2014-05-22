package calc;

import java.io.*;
import java.net.*;

import Jama.Matrix;

public class Server {
	public static void main(String[] args) throws InterruptedException {
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
			    while ( !connected ) {
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
			    System.out.println("Calculating minors  [" + startindex + " - " + endindex + "] ...");
			  
			    double result = 0.0;
			    int threads_count = 2;
			    Calculator []threads = new Calculator[threads_count];
				int start = startindex;
				int count = (int) Math.ceil((double)(endindex-startindex)/(double)(threads_count));
				int end = startindex+count-1;
				
				for (int i = 0; i < threads_count; i++) {
					System.out.println("Thread "+i+" ~ ["+start+" - "+end+"]");
					threads[i] = new Calculator(matrix, size, start, end, i);
					threads[i].start();
					start += count;
					end += count;
					if ( (end > endindex) || (i == threads_count - 1) )
						end = endindex ;
				}
				
				for (Calculator thread: threads) {
					thread.join();
					result += thread.getResult();
				}

			    System.out.println("Sending data ...");
			    msg =new Message(result);
			    outputStream.writeObject(msg);
			    tcpsocket.close();
			}
			
	
		    
			} catch (ClassNotFoundException e) {
				e.printStackTrace();
			} catch (UnknownHostException e) {
				e.printStackTrace();
			} catch (IOException e) {
				if (e.getClass().getSimpleName().equals("EOFException")) {
					System.err.println("Connection to client lost.");
					System.err.println("Next try to connect after 10 seconds.");
					Thread.sleep(10000);
				} else if (e.getMessage().equals("Broken pipe")) {
					System.err.println("Connection to client lost.");
					System.err.println("Next try to connect after 10 seconds.");
					Thread.sleep(10000);
				} else {
				e.printStackTrace();
				}
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	
}
