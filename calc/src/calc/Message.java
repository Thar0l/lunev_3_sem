package calc;


import java.io.Serializable;

import Jama.*;



public class Message implements Serializable{

	private static final long serialVersionUID = 8506899078391430169L;
	private Matrix matrix;
	private double result;
	private String text;
	private String type;
	
	public Message(Matrix matrix) {
		this.matrix = matrix;
		this.result = 0;
		this.text = null;
		this.type = "Matrix";
	}
	
	public Message(double result) {
		this.matrix = null;
		this.result = result;
		this.text = null;
		this.type = "Double";
	}
	
	public Message(String text) {
		this.matrix = null;
		this.result = 0;
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
}
