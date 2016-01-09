package graph;

import java.util.ArrayList;

import overlap.Overlap;
import overlap.Read;

public class Edge {

	private Vertex startVertex;
	private Vertex endVertex;
	private Read read;
	private Overlap overlap;
	private int beginIdx;
	private int endIdx;
	public boolean reduce;
	private ArrayList<Edge> containedEdges=new ArrayList<>();
	
	public String toDNA(){
		return read.get(beginIdx, endIdx);
	}
	
	public String toGFA(){
		return
				"L\t"+
				overlap.leftPart().getRead().getID()+
				"\t"+(overlap.leftPart().isForward()?"+":"-")+"\t"+
				overlap.rightPart().getRead().getID()+
				"\t"+(overlap.rightPart().isForward()?"+":"-")+"\t"+
				overlap.size()+"M";
	}
	
	@Override
	public String toString() {
		return startVertex+"->"+endVertex+" LEN:"+length();//"Len:"+length();
	//	return overlap.IDX+"";//+" LEN:"+length();
	}
	
	public ArrayList<Overlap> getOverlaps(){
		ArrayList<Overlap> overlaps=new ArrayList<>();
		overlaps.add(getOverlap());
		for(Edge edge:containedEdges){
			overlaps.addAll(edge.getOverlaps());
		}
		return overlaps;
	}
	
	public Overlap getOverlap(){
		return overlap;
	}
	
	public boolean hasContainedEdges(){
		return containedEdges.size()>0;
	}
	
	@Override
	public int hashCode() {
		return 31*startVertex.hashCode()+37*endVertex.hashCode();
	}
	
	public Vertex getStartVertex(){
		return startVertex;
	}
	
	public Read getRead(){
		return read;
	}

	public Vertex getEndVertex(){
		return endVertex;
	}
	
	private int segmentLength(){
		int l=beginIdx-endIdx;
		if(l<0){
			l=-l;
		}
		return l;
	}

	public int numContainedEdges(){
		int num=0;
		for(Edge edge:containedEdges){
			num+=1;
			num+=edge.numContainedEdges();
		}
		return num;
	}
	
	public int length(){
		int length=segmentLength();
		for(Edge edge:containedEdges){
			length+=edge.length();
		}
		return length;
	}
	
	public void remove(){
		startVertex.getOutEdges().remove(this);
		endVertex.getInEdges().remove(this);
	}
	
	public void addEdge(Edge edge){
		containedEdges.add(edge);
		endVertex.getInEdges().remove(this);
		endVertex=edge.getEndVertex();
		endVertex.getInEdges().add(this);
		edge.remove();
	}
	
	public Edge(Vertex startVertex,Vertex endVertex,Overlap overlap,Read read,int beginIdx,int endIdx){
		this.startVertex=startVertex;
		this.endVertex=endVertex;
		this.read=read;
		this.overlap=overlap;
		this.beginIdx=beginIdx;
		this.endIdx=endIdx;
		startVertex.getOutEdges().add(this);
		endVertex.getInEdges().add(this);
		startVertex.sortEdges();
		//System.out.println(this);
	}

	public boolean hasInteriorVertices() {
		return containedEdges.size()>0;
	}
	
}
