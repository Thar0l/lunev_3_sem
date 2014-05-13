package calc;


import java.io.Serializable;

import Jama.*;



public class Message implements Serializable{

	private static final long serialVersionUID = 8506899078391430169L;
	private Matrix matrix;
	private double result;
	private String text;
	private int type;
	
	public Message(Matrix matrix) {
		this.matrix = matrix;
		this.result = 0;
		this.text = null;
		this.type = 1;
	}
	
	public Message(double result) {
		this.matrix = null;
		this.result = result;
		this.text = null;
		this.type = 2;
	}
	
	public Message(String text) {
		this.matrix = null;
		this.result = 0;
		this.text = text;
		this.type = 3;
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
	
	public int getType() {
		return this.type;
	}
}
