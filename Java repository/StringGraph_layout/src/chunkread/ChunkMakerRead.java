package chunkread;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashSet;
import java.util.Iterator;

import graph.Graph;
import graph.Vertex;
import overlap.Overlap;
import overlap.Read;

public class ChunkMakerRead {

	private Graph graph;
	private ChunkRead chunkRead;
	private HashSet<Overlap> overlaps;
	private HashSet<Read> reads;
	public ArrayList<ChunkRead> chunkReads=new ArrayList<>();
	public ArrayList<ChunkRead> mergedChunks=new ArrayList<>();
	
	//method for building chunks by adding ChunkReads to it
	private boolean incrementChunk(){
		for(Overlap overlap:overlaps){
			if(chunkRead.canAddLeft(overlap)&&reads.contains(overlap.leftPart().getRead())){
				chunkRead.addLeft(overlap);
				reads.remove(overlap.leftPart().getRead());
				System.out.println("Added read:"+overlap.leftPart().getRead());
				return true;
			}

			if(chunkRead.canAddRight(overlap)&&reads.contains(overlap.rightPart().getRead())){
				chunkRead.addRight(overlap);
				reads.remove(overlap.rightPart().getRead());
				System.out.println("Added read:"+overlap.rightPart().getRead());
				return true;
			}
		}
		return false;
	}
	
	private void mergeChunks(){
		
	}
	
	//creating chunk reads by adding reads to chunkread; they are being sorted by their size
	public void process(){
		overlaps=graph.getOverlaps();
		reads=graph.getReads();
		while(reads.size()!=0){
			chunkRead=new ChunkRead();
			chunkReads.add(chunkRead);
			Iterator<Read> iterator=reads.iterator();
			Read first=iterator.next();
			iterator.remove();
			chunkRead.addRead(first);
			System.out.println("I");

			while(incrementChunk()){
			}
		}
		mergeChunks();
		chunkReads.sort(new Comparator<ChunkRead>() {

			@Override
			public int compare(ChunkRead o1, ChunkRead o2) {
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
	
	public ChunkMakerRead(Graph graph){
		this.graph=graph;
	}

}
