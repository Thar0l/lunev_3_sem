package calc;

public class TaskPart {
	private boolean done;
	private double result;
	private int startindex;
	private int endindex;
	
	public TaskPart(int startindex, int endindex) {
		this.startindex = startindex;
		this.endindex = endindex;
		this.done = false;
		this.result = 0;
	}
	
	public boolean isDone() {
		return this.done;
	}
	
	public void setDone() {
		this.done = true;
	}
	
	public int getStartIndex() {
		return this.startindex;
	}
	
	public int getEndIndex() {
		return this.endindex;
	}
	
	public double getResult() {
		return this.result;
	}
	
	public void setResult(double result) {
		this.result = result;
	}
}
