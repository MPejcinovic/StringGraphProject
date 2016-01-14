package chunk;

import java.util.ArrayList;

import graph.Edge;
import graph.Vertex;
import overlap.Overlap;
import overlap.OverlapPart;
import overlap.Read;

// class for working with chunks; every chunk has array of edges and vertices that are included in that chunk, as well as index of it

public class Chunk {

	public int idx;
	public ArrayList<Vertex> vertices=new ArrayList<>();
	public ArrayList<Edge> edges=new ArrayList<>();
	
	//method that writes out the information about vertices and edges included in chunk
	@Override
	public String toString() {
		int sum=0;
		StringBuilder b=new StringBuilder();
		b.append(vertices.get(0).getRead().getID()+" :"+vertices.get(0).getRead().length()+"\n");
		sum+=vertices.get(0).getRead().length();
		for(int i=0;i<edges.size();i++){
			//System.out.println("A");
			Edge edge=edges.get(i);
			if(edge.getStartVertex()!=vertices.get(i)){
				throw new IllegalStateException("IS");
			}
			sum+=edge.length();
			b.append(edge.toString()+" Sum:"+sum+"\n");
			/*
			Read read=edge.getEndVertex().getRead();
			Overlap overlap=edge.getOverlap();
			OverlapPart part=overlap.f;
			if(part.getRead()!=read){
				part=overlap.g;
			}

			if(part.isForward()){
				b.append(read.get(part.end, read.length()-1));
			}else{
				b.append(read.get(part.begin,0));
			}*/
		}
		return b.toString();
	}
	
	//last vertex in chunk
	public Vertex lastVertex(){
		return vertices.get(vertices.size()-1);
	}

	//first vertex in chunk
	public Vertex firstVertex(){
		return vertices.get(0);
	}


	//method for getting GFA format of chunks when they are represented in Gepard with 60 degrees inclination
	public String toGFA60(){
		StringBuilder b=new StringBuilder();
		for(Vertex v:vertices){
			b.append(v.getRead().sequence);
		}
		return b.toString();
	}
	
	
	//method for getting GFA format of chunks when they are represented in Gepard with 45 degrees inclination
	public String toGFA(){
		StringBuilder b=new StringBuilder();
		//b.append(vertices.get(0).getRead().sequence);
		if(!vertices.get(0).BEGIN){
			b.append(vertices.get(0).getRead().sequence);
		}else{
			b.append(Read.complement(vertices.get(0).getRead().sequence));
			b.reverse();
		}
		//b.append(vertices.get(0).getRead().sequence);
		for(int i=0;i<edges.size();i++){
			//System.out.println("A");
			Edge edge=edges.get(i);
			if(edge.getStartVertex()!=vertices.get(i)){
				throw new IllegalStateException("IS");
			}
			b.append(edge.toDNA());
			/*
			Read read=edge.getEndVertex().getRead();
			Overlap overlap=edge.getOverlap();
			OverlapPart part=overlap.f;
			if(part.getRead()!=read){
				part=overlap.g;
			}

			if(part.isForward()){
				b.append(read.get(part.end, read.length()-1));
			}else{
				b.append(read.get(part.begin,0));
			}*/
		}
		return b.toString();
	}
	
	public Chunk(){
		
	}

	public int size() {
		return toGFA().length();
	}
	
}

