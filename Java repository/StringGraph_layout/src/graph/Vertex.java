package graph;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.Iterator;
import java.util.List;

import overlap.Read;

public class Vertex implements Iterable<Edge>{

	private Read read;
	private boolean BEGIN;
	private List<Edge> outEdges=new ArrayList<>();
	private List<Edge> inEdges=new ArrayList<>();
	
	public void sortEdges(){
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
		//System.out.println(outEdges);
	}
	
	public Read getRead(){
		return read;
	}
	
	public static int hash(Read read,boolean BEGIN){
		return read.hashCode()*(BEGIN==true?1:-1);
	}
	
	@Override
	public int hashCode() {
		return hash(getRead(), BEGIN);
	}
	
	public List<Edge> getOutEdges(){
		return outEdges;
	}
	
	public List<Edge> getInEdges(){
		return inEdges;
	}
	
	public Vertex(Read read,boolean BEGIN) {
		this.read=read;
		this.BEGIN=BEGIN;
	}

	@Override
	public Iterator<Edge> iterator() {
		return outEdges.iterator();
	}
	
}
