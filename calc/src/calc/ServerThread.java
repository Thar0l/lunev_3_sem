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
			System.out.println("Server thread [" + number + "] run!");
			ObjectOutputStream outputStream = new ObjectOutputStream(tcpsocket.getOutputStream());
			ObjectInputStream inputStream   = new ObjectInputStream(tcpsocket.getInputStream());
		    
			
			
			
			
		    Message msg = new Message(Task.getMatrix(), Task.Parts.get(number).getStartIndex(), Task.Parts.get(number).getEndIndex(), Task.getMatrixSize());
		    //Message msg1 = new Message("Exit0");
		    outputStream.writeObject(msg);
		    msg = (Message) inputStream.readObject();
		   // outputStream.writeObject(msg1);
		    System.out.println("Server "+number+" calculated: "+msg.getResult());
		    Task.Parts.get(number).setResult(msg.getResult());
		    Task.Parts.get(number).setDone();
		    
		   // ServerConnectThread.clientscount--;
		    
		} catch (EOFException e) {
			// ServerConnectThread.clientscount--;
			 System.err.println("Server doing task [" + number + "] died!");
			 Task.Parts.get(number).setWorking(false);
		} catch (IOException e) {
			//ServerConnectThread.clientscount--;
			if (e.getMessage().equals("Connection reset")) {
				Task.Parts.get(number).setWorking(false);
				System.err.println("Server doing task [" + number + "] died!");
			} else {
			e.printStackTrace();
			}
		} catch (ClassNotFoundException e) {
			//ServerConnectThread.clientscount--;
			e.printStackTrace();
		}
	}
}
