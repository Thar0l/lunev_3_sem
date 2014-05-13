package calc;

import java.io.*;
import java.net.*;
import java.util.ArrayList;


public class Server {

	public static void main(String[] args) throws IOException, InterruptedException {
			
		BroadcastThread broadcastthread = new BroadcastThread("224.224.224.224", 4446);
		
		System.out.println("Broadcasting to clients...");
		
		broadcastthread.run();
		broadcastthread.join();
			
		System.out.println("Broadcast sended");
		System.out.println("Accepting client`s connections...");	
		
		
		ServerSocket socketListener = new ServerSocket (1234);
		ClientConnectThread.connected = false;
		ClientConnectThread.clientscount = 0;
		ClientConnectThread.Clients = new ArrayList<ClientThread>();
		ClientConnectThread cct = new ClientConnectThread(socketListener);
		cct.start();
		Thread.sleep(10000);
		socketListener.close();
		System.out.println("Connected " + ClientConnectThread.clientscount + " clients.");
	    for (ClientThread thread: ClientConnectThread.Clients) {
	    	thread.start();
	    }
	    for (ClientThread thread: ClientConnectThread.Clients) {
	    	thread.join();
	    }
		
	}

}
