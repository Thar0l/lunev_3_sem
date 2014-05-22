package calc;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;


public final class ServerConnectThread extends Thread{
	public static int clientscount;
	public static boolean connected; 
	public static ArrayList<ServerThread> Clients;
	private ServerSocket serversocket;
	private Task task;
	private boolean split;

	
	public ServerConnectThread(ServerSocket serversocket, Task task, boolean split) {
		this.serversocket = serversocket;
		this.task = task;
		this.split =split;
	}
	
	public void run() {
		try {
			while (!ServerConnectThread.connected) {
	            Socket server = null;
	            while (server == null) {
	            	server = serversocket.accept();
	            }
	            ServerThread servertthread= new ServerThread(server);
	            ServerConnectThread.Clients.add(servertthread);
	            ServerConnectThread.clientscount++;
	           
			}
		} catch (IOException e) {
			if (e.getMessage().equals("Accept timed out")) {
				System.out.println("Connection time out.");
				ServerConnectThread.connected = true;
				System.out.println("Connected " + ServerConnectThread.clientscount + " servers.");
				if (this.split)
					task.Split(ServerConnectThread.clientscount);
				
				
				
			    for (ServerThread thread: ServerConnectThread.Clients) {
			    	int partnumber = 0;
					for (int i = 0; i < Task.Parts.size(); i++) {
						if ( (!Task.Parts.get(i).isDone()) && (!Task.Parts.get(i).isWorking()) ) {
							partnumber = i;
							Task.Parts.get(partnumber).setWorking(true);
							break;
						}
					}
			    	thread.setTaskNumber(partnumber);
			    	thread.setTask(task);
			    	thread.start();
			    }
			} else
				e.printStackTrace();
		}
	}
		
}

