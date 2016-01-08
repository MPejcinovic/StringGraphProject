package overlap;

public class Read {

	private int ID;
	private int length;
		
	@Override
	public String toString() {
		return "{ID:"+ID+",length:"+length+"}";
	}
	
	@Override
	public boolean equals(Object obj) {
		Read r=(Read) obj;
		return r.ID==ID;
	}
	
	@Override
	public int hashCode() {
		return ID;
	}
	
	public int length(){
		return length;
	}
	
	public int getID(){
		return ID;
	}
	
	public Read(int ID,int length){
		this.ID=ID;
		this.length=length;
	}
	
}
