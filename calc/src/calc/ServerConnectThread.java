package calc;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;

import Jama.Matrix;

public final class ServerConnectThread extends Thread{
	public static int clientscount;
	public static boolean connected; 
	public static ArrayList<ServerThread> Clients;
	private ServerSocket serversocket;
	private Matrix matrix;
	
	public ServerConnectThread(ServerSocket serversocket, Matrix matrix) {
		this.serversocket = serversocket;
		this.matrix = matrix;
	}
	
	public void run() {
		try {
			while (!ServerConnectThread.connected) {
	            Socket server = null;
	            while (server == null) {
	            	server = serversocket.accept();
	            }
	            ServerThread servertthread= new ServerThread(server, ServerConnectThread.clientscount, matrix);
	            ServerConnectThread.Clients.add(servertthread);
	            ServerConnectThread.clientscount++;
	           
			}
		} catch (IOException e) {
			if (e.getMessage().equals("Accept timed out")) {
				System.out.println("Connection time out.");
				ServerConnectThread.connected = true;
				System.out.println("Connected " + ServerConnectThread.clientscount + " servers.");
			    for (ServerThread thread: ServerConnectThread.Clients) {
			    	thread.start();
			    }
			} else
				e.printStackTrace();
		}
	}
		
}

