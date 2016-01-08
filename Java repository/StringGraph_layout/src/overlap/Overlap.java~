package overlap;

public class Overlap {

	public int IDX;
	public OverlapPart f;
	public OverlapPart g;
		
	@Override
	public String toString() {
		return "{f:"+f+",g:"+g+"}";
	}
	
	public Overlap(int IDX,OverlapPart f,OverlapPart g){
		this.IDX=IDX;
		this.f=f;
		this.g=g;
	}
	
	public boolean containsRead(Read read){
		return f.containsRead(read)||g.containsRead(read);
	}
	
	public Read getContainedRead(){
		if(f.isContainment()){
			return f.getRead();
		}
		if(g.isContainment()){
			return g.getRead();
		}
		return null;
	}
	
}
