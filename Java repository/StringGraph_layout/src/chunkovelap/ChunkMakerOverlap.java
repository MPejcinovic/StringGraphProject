package chunkovelap;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashSet;
import java.util.Iterator;

import graph.Graph;
import overlap.Overlap;

public class ChunkMakerOverlap {

	private Graph graph;
	private ChunkOverlap chunkOverlap;
	private HashSet<Overlap> overlaps;
	public ArrayList<ChunkOverlap> chunkOverlaps=new ArrayList<>();
	public ArrayList<ChunkOverlap> mergedChunks=new ArrayList<>();
	
	private boolean incrementChunk(){
		Iterator<Overlap> iterator=overlaps.iterator();
		while(iterator.hasNext()){
			Overlap overlap=iterator.next();
			if(chunkOverlap.canAddLeft(overlap)){
				iterator.remove();
				chunkOverlap.addLeft(overlap);
				return true;
			}

			if(chunkOverlap.canAddRight(overlap)){
				iterator.remove();
				chunkOverlap.addRight(overlap);
				return true;
			}
		}
		return false;
	}
	
	private void mergeChunks(){
		
	}
	
	public void process(){
		overlaps=graph.getOverlaps();
		while(overlaps.size()!=0){
			chunkOverlap=new ChunkOverlap();
			chunkOverlaps.add(chunkOverlap);
			while(incrementChunk()){
			}
		}
		mergeChunks();
		chunkOverlaps.sort(new Comparator<ChunkOverlap>() {

			@Override
			public int compare(ChunkOverlap o1, ChunkOverlap o2) {
				if(o1.size()<o2.size()){
					return 1;
				}
				if(o1.size()==o2.size()){
					return 0;
				}
				return -1;
			}
		});
		
	}
	
	public ChunkMakerOverlap(Graph graph){
		this.graph=graph;
	}
	
}
