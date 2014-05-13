package calc;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.*;

public class ClientThread extends Thread {
	private Socket tcpsocket;
	private int number;
	
	public ClientThread (Socket socket, int nubmer) {
		this.tcpsocket = socket;
		this.number = nubmer;
	}
	
	public void run() {
		try {
			System.out.println("Client thread[" + number + "] run!");
			ObjectOutputStream outputStream = new ObjectOutputStream(tcpsocket.getOutputStream());
			ObjectInputStream inputStream   = new ObjectInputStream(tcpsocket.getInputStream());
		    
		    Message msg = null;
		    msg = (Message) inputStream.readObject();
		    outputStream.writeObject(msg);
		} catch (IOException e) {
			e.printStackTrace();
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
	}
}
