package calc;

import java.io.*;
import java.net.*;
import java.util.*;
import Jama.*;


public class Client {

	public static void main(String[] args) throws IOException, InterruptedException {
			
		int size = 500;
		
		Matrix matrix = Matrix.random(size, size);
		
		Task task = new Task(matrix, size);
		Task.initResult();
		int needtodo = 0;
		boolean split = true;
		
		MulticastThread multicastthread; 
		ServerSocket socketListener; 

	    while (needtodo > 0) {
	    	multicastthread = new MulticastThread("224.224.224.224", 4444);
	    	socketListener = new ServerSocket (1234);
			socketListener.setSoTimeout(800);
			ServerConnectThread.connected = false;
			ServerConnectThread.clientscount = 0;
			ServerConnectThread.Clients = new ArrayList<ServerThread>();
			ServerConnectThread sct2 = new ServerConnectThread(socketListener, task, split);
			sct2.start();
			System.out.println("Broadcasting to servers...");
			multicastthread.run();
			multicastthread.join();
			System.out.println("Broadcast sended");
			System.out.println("Accepting server`s connections...");
			sct2.join();
		    for (ServerThread thread: ServerConnectThread.Clients) {
		    	thread.join();
		    needtodo=0;
			for (TaskPart p: Task.Parts) {
				  if (!p.isDone()) needtodo++;
			}
		    }
		    socketListener.close();
		    if (split) split = false;

	    }
	    task.calcResult();
	    System.out.println("Result = "+Task.getResult());
	} 

}
