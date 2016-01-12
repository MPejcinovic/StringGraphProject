package chunk;

import java.util.ArrayList;

import graph.Edge;
import graph.Vertex;
import overlap.Overlap;
import overlap.OverlapPart;
import overlap.Read;

public class Chunk {

	public int idx;
	public ArrayList<Vertex> vertices=new ArrayList<>();
	public ArrayList<Edge> edges=new ArrayList<>();
	
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
	
	public Vertex lastVertex(){
		return vertices.get(vertices.size()-1);
	}

	public Vertex firstVertex(){
		return vertices.get(0);
	}

	public String toGFA60(){
		StringBuilder b=new StringBuilder();
		for(Vertex v:vertices){
			b.append(v.getRead().sequence);
		}
		return b.toString();
	}
	
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

