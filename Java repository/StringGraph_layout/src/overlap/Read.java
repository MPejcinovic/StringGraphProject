package overlap;

public class Read {

	private int ID;
	private int length;
	public String sequence;
	
	public static String complement(String str){
		StringBuilder b=new StringBuilder(str);
		for(int i=0;i<b.length();i++){
			if(b.charAt(i)=='A'){
				b.setCharAt(i, 'T');
				continue;
			}
			if(b.charAt(i)=='G'){
				b.setCharAt(i, 'C');
				continue;
			}
			if(b.charAt(i)=='C'){
				b.setCharAt(i, 'G');
				continue;
			}
			if(b.charAt(i)=='T'){
				b.setCharAt(i, 'A');
				continue;
			}
		}
		return b.toString();
	}
	
	public String get(int begin,int end){
		if(begin<end){
			return sequence.substring(begin, end+1);
		}
		if(begin==end){
			return "";
		}
		//System.out.println("B:"+begin+" E:"+end);
		return complement(sequence.substring(end+1, begin));
	}
	
	public String toGFA(){
		return "S\t"+getID()+"\t"+sequence;
	}

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

