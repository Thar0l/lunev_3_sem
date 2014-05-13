package calc;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;

public final class ClientConnectThread extends Thread{
	public static int clientscount;
	public static boolean connected; 
	public static ArrayList<ClientThread> Clients;
	private ServerSocket serversocket;
	
	public ClientConnectThread(ServerSocket serversocket) {
		this.serversocket = serversocket;
	}
	
	public void run() {
		try {
			while (!ClientConnectThread.connected) {
	            Socket client = null;
	            while (client == null) {
	                client = serversocket.accept();
	            }
	            ClientThread clientthread= new ClientThread(client, ClientConnectThread.clientscount);
	            ClientConnectThread.Clients.add(clientthread);
	            ClientConnectThread.clientscount++;
	           
			}
		} catch (IOException e) {
			if (e.getMessage().equals("Socket closed")) {
				System.out.println("Connection time out.");
				ClientConnectThread.connected = true;
			} else
				e.printStackTrace();
		}
	}
		
}

