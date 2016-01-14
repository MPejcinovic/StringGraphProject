package chunk;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashSet;
import java.util.Iterator;

import graph.Edge;
import graph.Graph;
import graph.Vertex;
import overlap.Overlap;
import overlap.Read;

public class ChunkMaker {

	private static final int MIN_LENGTH=800;
	private static final int MAX_LENGTH=14000;
	private Chunk currentChunk;
	private Graph graph;
	private HashSet<Read> unusedReads;
	public ArrayList<Vertex> vertices;
	public ArrayList<Chunk> chunks=new ArrayList<>();
	
	//method for adding vertex
	private void vertexAdded(Vertex v){
		int r=0;
		for(int i=0;i<vertices.size();i++){
			if(vertices.get(i).getRead()==v.getRead()){
				vertices.remove(i);
				i--;
				r++;
			}
		}
	}
	
	//adding vertex to the end of chunk 
	private void addToCurrentChunkEnd(Vertex v){
		currentChunk.vertices.add(v);
		vertexAdded(v);
	}

	//adding vertex to the beginning of chunk 
	private void addToCurrentChunkBegin(Vertex v){
		currentChunk.vertices.add(0,v);
		vertexAdded(v);
	}

	//method for making chunks; adding edges and vertices to chunk
	private boolean incrementChunk(){
		ArrayList<Edge> outEdges=new ArrayList<>(currentChunk.lastVertex().getOutEdges());
		outEdges.sort(new Comparator<Edge>() {

			@Override
			public int compare(Edge o1, Edge o2) {
				if(o1.length()<o2.length()){
					return -1;
				}
				if(o1.length()==o2.length()){
					return 0;
				}
				return 1;
			}
		});

		if(true){
			for(Edge e:outEdges){
				int len=e.segmentLength();
				if(len>MAX_LENGTH){
					continue;
				}
				if(!vertices.contains(e.getEndVertex())){
					continue;
				}
				addToCurrentChunkEnd(e.getEndVertex());
				currentChunk.edges.add(e);
				return true;
			}
		}
		
		ArrayList<Edge> inEdges=new ArrayList<>(currentChunk.firstVertex().getInEdges());
		inEdges.sort(new Comparator<Edge>() {

			@Override
			public int compare(Edge o1, Edge o2) {
				if(o1.length()<o2.length()){
					return -1;
				}
				if(o1.length()==o2.length()){
					return 0;
				}
				return 1;
			}
		});
		if(true){
			for(Edge e:inEdges){
				int len=e.segmentLength();
				if(len>MAX_LENGTH){
					continue;
				}
				if(!vertices.contains(e.getStartVertex())){
					continue;
				}
				addToCurrentChunkBegin(e.getStartVertex());
				currentChunk.edges.add(0,e);
				return true;
			}

		}

		return false;
	}
		
	public void process(){
		unusedReads=new HashSet<>(graph.getReads());
		vertices=new ArrayList<>(graph.getVertices());
		while(vertices.size()!=0){
			int maxIdx=0;
			int maxLen=-1;
			ArrayList<Vertex> vCopy=new ArrayList<>(vertices);
			for(int i=0;i<vertices.size();i++){
				currentChunk=new Chunk();
				addToCurrentChunkEnd(vertices.get(i));
				while(incrementChunk()){
				}
				int size=currentChunk.size();
				if(maxLen==-1||size>maxLen){
					maxIdx=i;
					maxLen=size;
				}
				vertices=new ArrayList<>(vCopy);
			}
			vertices=new ArrayList<>(vCopy);
			currentChunk=new Chunk();
			addToCurrentChunkEnd(vertices.get(maxIdx));
			while(incrementChunk()){
			}
			chunks.add(currentChunk);
			System.out.println(maxIdx);
			
			//break;
		}
		chunks.sort(new Comparator<Chunk>() {

			@Override
			public int compare(Chunk o1, Chunk o2) {
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
	
	public ChunkMaker(Graph graph){
		this.graph=graph;
	}
	
}
