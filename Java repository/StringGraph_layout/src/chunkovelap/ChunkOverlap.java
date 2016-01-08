package chunkovelap;

import java.util.ArrayList;

import overlap.Overlap;

public class ChunkOverlap {
	
	private ArrayList<Overlap> overlaps=new ArrayList<>();
	
	public int size(){
		int size=0;
		for(Overlap overlap:overlaps){
			size+=overlap.size();
		}
		return size;
	}
	
	public boolean isEmpty(){
		return overlaps.isEmpty();
	}
	
	public void addRight(Overlap overlap){
		overlaps.add(overlap);
	}
	
	public boolean canAddRight(Overlap overlap){
		if(isEmpty()){
			return true;
		}
		Overlap last=overlaps.get(overlaps.size()-1);
		if(last.rightPart().getRead()==overlap.leftPart().getRead()){
			return true;
		}
		return false;
	}

	public void addLeft(Overlap overlap){
		overlaps.add(0,overlap);
	}
	
	public boolean canAddLeft(Overlap overlap){
		if(isEmpty()){
			return true;
		}
		Overlap first=overlaps.get(0);
		if(first.leftPart().getRead()==overlap.rightPart().getRead()){
			return true;
		}
		return false;
	}

	public ChunkOverlap(){
		
	}
	
}
