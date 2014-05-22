package calc;


import java.io.Serializable;

import Jama.*;



public class Message implements Serializable{

	private static final long serialVersionUID = 8506899078391430169L;
	private Matrix matrix;
	private int startminorindex;
	private int endminorindex;
	private int size;
	private double result;
	private String text;
	private String type;
	
	public Message(Matrix matrix, int startminorindex, int endminorindex, int size) {
		this.matrix = matrix;
		this.startminorindex = startminorindex;
		this.endminorindex = endminorindex;
		this.size = size;
		this.result = 0;
		this.text = "-";
		this.type = "Matrix";
	}
	
	public Message(double result) {
		this.matrix = null;
		this.result = result;
		this.startminorindex = 0;
		this.endminorindex = 0;
		this.size = 0;
		this.text = "-";
		this.type = "Double";
	}
	
	public Message(String text) {
		this.matrix = null;
		this.result = 0;
		this.startminorindex = 0;
		this.endminorindex = 0;
		this.size = 0;
		this.text = text;
		this.type = "String";
	}
	
	public String getText() {
		return this.text;
	}
	
	public double getResult() {
		return this.result;
	}
	
	public Matrix getMatrix() {
		return this.matrix;
	}
	
	public String getType() {
		return this.type;
	}
	
	public int getStartMinorIndex() {
		return this.startminorindex;
	}
	
	public int getEndMinorIndex() {
		return this.endminorindex;
	}
	
	public int getMatrixSize() {
		return this.size;
	}
	
}
