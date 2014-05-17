package calc;

import java.io.*;
import java.net.*;
import java.util.*;
import Jama.*;


public class Client {

	public static void main(String[] args) throws IOException, InterruptedException {
			
		
		Matrix matrix = Matrix.random(300, 300);
		
		BroadcastThread broadcastthread = new BroadcastThread("224.224.224.224", 4446);
		ServerSocket socketListener = new ServerSocket (1234);
		socketListener.setSoTimeout(3000);
		ServerConnectThread.connected = false;
		ServerConnectThread.clientscount = 0;
		ServerConnectThread.Clients = new ArrayList<ServerThread>();
		ServerConnectThread cct = new ServerConnectThread(socketListener, matrix);
		cct.start();
		System.out.println("Broadcasting to servers...");
		broadcastthread.run();
		broadcastthread.join();
		System.out.println("Broadcast sended");
		System.out.println("Accepting server`s connections...");	
	    for (ServerThread thread: ServerConnectThread.Clients) {
	    	thread.join();
	    }
		
	} 

}
