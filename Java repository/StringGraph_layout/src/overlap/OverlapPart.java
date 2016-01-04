package overlap;

public class OverlapPart {
	public static double percentMargin=1.6;
	public static int absoluteMargin=10;

	private Read read;
	public final int begin;
	public final int end;
	
	@Override
	public String toString() {
		return "{read:"+read+"begin:"+begin+",end:"+end+"}";
	}
	
	public boolean isForward(){
		return begin<=end;
	}
	
	public OverlapPart(Read read,int begin,int end){
		this.read=read;
		this.begin=begin;
		this.end=end;
	}
	
	public Read getRead(){
		return read;
	}
	
	public int getLength(){
		int length=end-begin;
		if(length<0){
			length=-length;
		}
		return length;
	}
	
	public boolean isContainment(){
		int length=getLength();
		return length>=read.length()*(1-percentMargin/100)-absoluteMargin;
	}

	public boolean containsRead(Read read) {
		return this.read==read;
	}
	
}
