package overlap;

public class OverlapPart {
	//parameter for better definition which overlap is containment
	public static double percentMargin=7;
	
	private Read read;
	public final int begin;
	public final int end;
	public final int originalBegin;
	public final int originalEnd;
	
	//get complement of part that is not overlap
	public String getOverhangComplemented(){
		return Read.complement(getOverhang());
	}

	//setting extremes for overlap; calculating overhangs 
	public String getOverhang(){
		if(isForward()){
			if(begin==0){
				return read.get(originalEnd+1, read.length()-1);
			}
			return read.get(0, originalBegin-1);
		}
		if(end==0){
			return read.get(read.length()-1, originalBegin+1);
		}
		return read.get(originalEnd-1,0);
	}
	
	@Override
	public String toString() {
		return "{read:"+read+"begin:"+begin+",end:"+end+"}";
	}
	
	// defining direction
	public boolean isForward(){
		return begin<=end;
	}
	
	public boolean isLeftPart(){
		return isForward()&&begin!=0||!isForward()&&begin!=read.length()-1;
	}
	
	public boolean isRightPart(){
		return !isLeftPart();
	}
	
	public OverlapPart(Read read,int begin,int end, int originalBegin, int originalEnd){
		this.read=read;
		this.begin=begin;
		this.end=end;
		this.originalBegin=originalBegin;
		this.originalEnd=originalEnd;
	}
	
	public Read getRead(){
		return read;
	}
	
	public int length(){
		int length=end-begin;
		if(length<0){
			length=-length;
		}
		length++;
		return length;
	}

	//condition for containment	
	public boolean isContainment(){
		return //length()==read.length();
				length()>=read.length()*(1-percentMargin/100);
	}

	public boolean containsRead(Read read) {
		return this.read==read;
	}
	
}
