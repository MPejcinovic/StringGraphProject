package graph;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;

import overlap.Overlap;
import overlap.Read;

public class Graph implements Iterable<Vertex>{

	private List<Vertex> vertices;
	
	public HashSet<Overlap> getOverlaps(){
		HashSet<Overlap> arr=new HashSet<>();
		for(Vertex v:this){
			for(Edge e:v){
				arr.add(e.getOverlap());
			}
		}
		return arr;
	}
	
	public int numVertices(){
		return vertices.size();
	}
	
	public int numEdges(){
		int s=0;
		for(Vertex v:vertices){
			s+=v.getOutEdges().size();
		}
		return s;
	}
	
	@Override
	public String toString() {
		return "|V|="+numVertices()+",|E|="+numEdges();
	}
	
	public List<Vertex> getVertices(){
		return vertices;
	}
	
	private void createVertices(Collection<Read> reads){
		vertices=new ArrayList<>();
		for(Read r:reads){
			Vertex B=new Vertex(r, true);
			Vertex E=new Vertex(r, false);
			vertices.add(B);
			vertices.add(E);
		}
	}
	
	private void createEdge(Overlap overlap,HashMap<Integer, Vertex> map){
		Read f=overlap.f.getRead();
		Read g=overlap.g.getRead();
		Vertex FB=map.get(Vertex.hash(f, true));
		Vertex FE=map.get(Vertex.hash(f, false));
		Vertex GB=map.get(Vertex.hash(g, true));
		Vertex GE=map.get(Vertex.hash(g, false));
		if(overlap.f.begin>=overlap.f.end){
			System.out.println(overlap.f.begin);
			System.out.println(overlap.f.getLength());
			System.out.println("EEEEE");
		}
		if(overlap.f.begin>0){
			if(overlap.g.begin<overlap.g.end){
				new Edge(GB, FB,overlap, f, overlap.f.begin, 0);
				new Edge(FE, GE,overlap, g, overlap.g.end, overlap.g.getLength());
			}else{
				new Edge(GE, FB,overlap, f, overlap.f.begin, 0);
				new Edge(FE, GB, overlap,g, overlap.g.end, 0);
			}
		}
		else{
			if(overlap.g.begin<overlap.g.end){
				new Edge(FB, GB, overlap,g, overlap.g.begin, 0);
				new Edge(GE, FE, overlap,f, overlap.f.end, overlap.f.getLength());
			}else{
				new Edge(FB, GE,overlap, g, overlap.g.getLength(), overlap.g.begin);
				new Edge(GB, FE, overlap,f, overlap.f.end, overlap.f.getLength());
			}
		}
	}
	
	private void createEdges(Collection<Overlap> overlaps){
		HashMap<Integer, Vertex> map=new HashMap<>();
		for(Vertex v:vertices){
			map.put(v.hashCode(), v);
		}
		for(Overlap overlap:overlaps){
			createEdge(overlap, map);
		}
	}
	
	public void removeEmptyVertices(){
		Iterator<Vertex> it=vertices.iterator();
		while(it.hasNext()){
			Vertex v=it.next();
			if(v.getInEdges().isEmpty()&&v.getOutEdges().isEmpty()){
				it.remove();
			}
		}
	}
	
	public Graph(Collection<Read> reads,Collection<Overlap> overlaps){
		createVertices(reads);
		createEdges(overlaps);
		removeEmptyVertices();
		for(Vertex v:vertices){
			v.sortEdges();
		}
	}

	@Override
	public Iterator<Vertex> iterator() {
		return vertices.iterator();
	}
	
}
