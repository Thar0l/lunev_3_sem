package calc;

import java.io.*;
import java.net.*;
import java.util.*;

import Jama.*;


public class Client {

	public static void main(String[] args) throws IOException, InterruptedException {
			
		int size = 600;
		
		Matrix matrix = Matrix.random(size, size);
		
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++) 
				matrix.set(i, j, matrix.get(i,j)/10 + 1.1);
		
		Task task = new Task(matrix, size);
		Task.initResult();
		int remainingtaskscount = 1;
		boolean split = true;
		MulticastThread multicastthread; 
		ServerSocket socketListener; 

	    while ( remainingtaskscount > 0 ) {
	    	
	    	multicastthread = new MulticastThread("224.224.224.224", 4444);
	    	socketListener = new ServerSocket (1234);
			socketListener.setSoTimeout(1200);
			ServerConnectThread.connected = false;
			ServerConnectThread.serverscount = 0;
			ServerConnectThread.Clients = new ArrayList<ServerThread>();
			ServerConnectThread sct = new ServerConnectThread(socketListener, task, split);
			sct.start();
			System.out.println("Searching for servers...");
			multicastthread.run();
			multicastthread.join();
			System.out.println("Broadcast sended");
			System.out.println("Accepting server`s connections...");
			sct.join();
		    for (ServerThread thread: ServerConnectThread.Clients) {
		    	thread.join();
		    }
		    remainingtaskscount = 0;
			for (int i = 0; i < Task.Parts.size(); i++) {
				  if ( !Task.Parts.get(i).isDone() ) remainingtaskscount++;
				}
		    
		    socketListener.close();
		    if ( split ) split = false;
		    if ( remainingtaskscount == 0 )
		    	System.out.println("All tasks done.");
		    else
		    	System.out.println("Remaining " + remainingtaskscount + " tasks.");
	    }
	    task.calcResult();
	    System.out.println("Result = "+Task.getResult());
	} 

}
