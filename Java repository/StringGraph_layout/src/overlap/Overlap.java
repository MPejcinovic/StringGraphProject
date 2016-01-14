package overlap;

public class Overlap {

	public int IDX;
	public OverlapPart f;
	public OverlapPart g;
			
	//getting overhangs for overlap; parts that are on the left and right side of the overlap
	public String getLeftOverhang(){
		return leftPart().getOverhang();
	}
	
	public String getRightOverhang(){
		return rightPart().getOverhang();
	}
	
	@Override
	public String toString() {
		return "{f:"+f+",g:"+g+"}";
	}
	
	public OverlapPart leftPart(){
		if(f.isLeftPart()){
			return f;
		}
		return g;
	}

	public OverlapPart rightPart(){
		if(f.isRightPart()){
			return f;
		}
		return g;
	}

	public Overlap(int IDX,OverlapPart f,OverlapPart g){
		this.IDX=IDX;
		this.f=f;
		this.g=g;
	}
	
	public boolean containsRead(Read read){
		return f.containsRead(read)||g.containsRead(read);
	}
	
	private static int i=0;

	//returning reads that are contained in graph	
	public Read getContainedRead(){
		if(g.isContainment()&&f.isContainment()){
			i++;
			System.err.println("I:"+i);
			return null;
		}
		if(f.isContainment()){
			return f.getRead();
		}
		if(g.isContainment()){
			return g.getRead();
		}
		return null;
	}

	public int size() {
		return f.length();
	}
	
}
