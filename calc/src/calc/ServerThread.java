package calc;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.*;

import Jama.Matrix;

public class ServerThread extends Thread {
	private Socket tcpsocket;
	private int number;
	private Matrix matrix;
	
	public ServerThread (Socket socket, int nubmer, Matrix matrix) {
		this.tcpsocket = socket;
		this.number = nubmer;
		this.matrix = matrix;
	}
	
	public void run() {
		try {
			System.out.println("Server thread[" + number + "] run!");
			ObjectOutputStream outputStream = new ObjectOutputStream(tcpsocket.getOutputStream());
			ObjectInputStream inputStream   = new ObjectInputStream(tcpsocket.getInputStream());
		    
		    Message msg = new Message(matrix);
		    outputStream.writeObject(msg);
		    msg = (Message) inputStream.readObject();
		    System.out.println("Server "+number+" calculated: "+msg.getResult());
		    
		} catch (IOException e) {
			if (e.getMessage().equals("Connection reset")) {
				System.err.println("Server died!");
			} else {
			e.printStackTrace();
			}
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
	}
}
