package calc;

import java.io.EOFException;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.*;


public class ServerThread extends Thread {
	private Socket tcpsocket;
	private int number;
	private Task task;

	
	public ServerThread (Socket socket) {
		this.tcpsocket = socket;
	}
	
	public void setTaskNumber(int number) {
		this.number = number;
	}
	
	public void setTask(Task task) {
		this.task = task;
	}
	
	public Task getTask() {
		return this.task;
	}
	

	public synchronized void run() {
		try {
			System.out.println("Server thread for task {" + number + "} started.");
			
			ObjectOutputStream outputStream = new ObjectOutputStream(tcpsocket.getOutputStream());
			ObjectInputStream inputStream   = new ObjectInputStream(tcpsocket.getInputStream());
		    			
		    Message msg = new Message(Task.getMatrix(), Task.Parts.get(number).getStartIndex(), Task.Parts.get(number).getEndIndex(), Task.getMatrixSize());
		    outputStream.writeObject(msg);
		    msg = (Message) inputStream.readObject();
		    Task.Parts.get(number).setResult(msg.getResult());
		    Task.Parts.get(number).setDone();
		    
		    System.out.println("Server calculated task (" + number + "). Result = : "+msg.getResult());
		    if ( Task.Parts.get(number).isDone() )
		    	System.out.println("Task {" + number + "} is done.");
		    else
		    	System.out.println("Task {" + number + "} isn`t done.");
		    		    	
		} catch (SocketException e) {
				Task.Parts.get(number).setWorking(false);
				System.err.println("Server doing task {" + number + "} disconnected!");
		} catch (EOFException e) {
			System.err.println("Server doing task {" + number + "} disconnected!");
			Task.Parts.get(number).setWorking(false);
		} catch (IOException e) {
			if (e.getMessage().equals("Connection reset")) {
				Task.Parts.get(number).setWorking(false);
				System.err.println("Server doing task {" + number + "} disconnected!");
			} else {
			e.printStackTrace();
			}
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
	}
}
