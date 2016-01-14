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

	public String toGFA(){
		StringBuilder b=new StringBuilder();
		b.append("H\tVN:Z:1.0\n");
		for(Read r:getReads()){
			b.append(r.toGFA());
			b.append("\n");
		}
		for(Vertex v:vertices){
			for(Edge e:v){
				b.append(e.toGFARecursive());
				b.append("\n");
			}
		}
		return b.toString();
	}
	
	public HashSet<Read> getReads(){
		HashSet<Read> arr=new HashSet<>();
		for(Vertex v:this){
			for(Edge e:v){
				for(Overlap overlap:e.getOverlaps()){
					arr.add(overlap.f.getRead());
					arr.add(overlap.g.getRead());
				}
			}
		}
		return arr;
	}
	
	public HashSet<Overlap> getOverlaps(){
		HashSet<Overlap> arr=new HashSet<>();
		for(Vertex v:this){
			for(Edge e:v){
				arr.addAll(e.getOverlaps());
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
			//new Edge(B, E, null, r, 0, r.length());
			//new Edge(E,B, null, r, 0, r.length());
		}
	}
	
	int i1=0,i2=0,i3=0,i4=0;

	private void createEdge(Overlap overlap,HashMap<Integer, Vertex> map){
		Read f=overlap.f.getRead();
		Read g=overlap.g.getRead();
		Vertex FB=map.get(Vertex.hash(f, true));
		Vertex FE=map.get(Vertex.hash(f, false));
		Vertex GB=map.get(Vertex.hash(g, true));
		Vertex GE=map.get(Vertex.hash(g, false));
		if(overlap.f.begin>=overlap.f.end){
			System.out.println(overlap.f.begin);
			System.out.println(overlap.f.length());
			System.out.println("EEEEE");
		}
		if(overlap.f.begin>0){
			if(overlap.g.begin<overlap.g.end){
				//System.out.println("I1:"+overlap);
				i1++;
				new Edge(GB, FB,overlap, f, overlap.f.begin, 0,1);
				new Edge(FE, GE,overlap, g, overlap.g.end, g.length(),1);
			}else{
				//System.out.println("I2:"+overlap);
				i2++;
				new Edge(GE, FB,overlap, f, overlap.f.begin, 0,21);
				new Edge(FE, GB, overlap,g, overlap.g.end, 0,22);
			}
		}
		else{
			if(overlap.g.begin<overlap.g.end){
				//System.out.println("I3:"+overlap);
				i3++;
				new Edge(FB, GB, overlap,g, overlap.g.begin, 0,3);
				new Edge(GE, FE, overlap,f, overlap.f.end, f.length(),3);
			}else{
				//System.out.println("I4:"+overlap);
				i4++;
				new Edge(FB, GE,overlap, g, overlap.g.begin,g.length(),4);
				new Edge(GB, FE, overlap,f, overlap.f.end, f.length(),4);
			}
		}
	}
	
	private void createEdges(Collection<Overlap> overlaps){
		HashMap<Integer, Vertex> map=new HashMap<>();
		for(Vertex v:vertices){
			if(map.get(v.hashCode())!=null){
				throw new IllegalStateException("map entry != null");
			}
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
		System.out.println("I1:"+i1);
		System.out.println("I2:"+i2);
		System.out.println("I3:"+i3);
		System.out.println("I4:"+i4);
		removeEmptyVertices();
		for(Vertex v:vertices){
			v.sortEdges();
		}
	}

	@Override
	public Iterator<Vertex> iterator() {
		return vertices.iterator();
	}

	public void report() {
		for(Vertex v:vertices){
			for(Edge e:v){
				e.getOverlap().f.isLeftPart();
				e.getOverlap().g.isLeftPart();
			}
		}
	}
	
}
