package calc;

import java.util.ArrayList;

import Jama.Matrix;

public class Task {
	private int size;
	private Matrix matrix;
	public ArrayList<TaskPart> Parts;
	private double result;
	
	public Task(Matrix matrix, int size) {
		this.matrix = matrix;
		this.size = size;
		this.result = 0;
	}
	
	public void Split(int partscount) {
		int partsize = size/partscount;
		this.Parts = new ArrayList<TaskPart>();
		for (int i = 0; i< partscount; i++) 
			Parts.add(new TaskPart(0, partsize));
	}
	
	public Matrix getMatrix() {
		return this.matrix;
	}
	
	public double getResult() {
		return this.result;
	}
	
	public void calcResult() {
		for (int i = 0; i< Parts.size(); i++)
			this.result = this.result + this.Parts.get(i).getResult();
	}

}
