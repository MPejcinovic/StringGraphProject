package chunkread;

import java.util.ArrayList;

import overlap.Overlap;
import overlap.Read;

public class ChunkRead {

	public int idx;
	public ArrayList<Read> reads=new ArrayList<>();
	public ArrayList<Overlap> overlaps=new ArrayList<>();
	
	public String toGFA(){
		StringBuilder b=new StringBuilder("S\t"+idx+"\t");
		b.append(reads.get(0).sequence);
		boolean lastIsForward=false;
		for(int i=1;i<reads.size();i++){
			Overlap overlap=overlaps.get(i-1);
			if(overlap.leftPart().isForward()!=lastIsForward){
				b.append(Read.complement(overlap.getRightOverhang()));
			}else{
				b.append(overlap.getRightOverhang());
			}
			lastIsForward=overlap.leftPart().isForward();
		}
		b.append("\n");
		return b.toString();
	}
	
	public int size(){
		return toGFA().length();
	}
	
	public boolean isEmpty(){
		return reads.isEmpty();
	}
	
	public void addRight(Overlap overlap){
		reads.add(overlap.rightPart().getRead());
		overlaps.add(overlap);
	}
	
	public boolean canAddRight(Overlap overlap){
		if(isEmpty()){
			return true;
		}
		Read last=reads.get(reads.size()-1);
		if(last==overlap.leftPart().getRead()){
			return true;
		}
		return false;
	}

	public void addLeft(Overlap overlap){
		reads.add(0,overlap.leftPart().getRead());
		overlaps.add(0,overlap);
	}
	
	public boolean canAddLeft(Overlap overlap){
		if(isEmpty()){
			return true;
		}
		Read first=reads.get(0);
		if(first==overlap.rightPart().getRead()){
			return true;
		}
		return false;
	}

	public ChunkRead(){
		
	}

	public void addRead(Read first) {
		reads.add(first);
	}
	
}

