package chunk;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashSet;
import java.util.Iterator;

import graph.Edge;
import graph.Graph;
import graph.Vertex;
import overlap.Overlap;

public class ChunkMaker {

	private Chunk currentChunk;
	private Graph graph;
	public ArrayList<Vertex> vertices;
	public ArrayList<Chunk> chunks=new ArrayList<>();
	
	private boolean incrementChunk(){
		for(Edge e:currentChunk.lastVertex()){
			if(!vertices.contains(e.getEndVertex())){
				continue;
			}
			currentChunk.vertices.add(e.getEndVertex());
			currentChunk.edges.add(e);
			vertices.remove(e.getEndVertex());
			return true;
		}
		
		/*for(Edge e:currentChunk.firstVertex().getInEdges()){
			if(!vertices.contains(e.getStartVertex())){
				continue;
			}
			currentChunk.vertices.add(0,e.getStartVertex());
			currentChunk.edges.add(0,e);
			vertices.remove(e.getStartVertex());
			return true;
		}*/

		return false;
	}
		
	public void process(){
		vertices=new ArrayList<>(graph.getVertices());
		while(vertices.size()!=0){
			currentChunk=new Chunk();
			currentChunk.vertices.add(vertices.get(0));
			chunks.add(currentChunk);
			vertices.remove(0);
			while(incrementChunk()){
			}
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
