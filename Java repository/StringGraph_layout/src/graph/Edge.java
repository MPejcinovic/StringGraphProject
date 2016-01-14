package graph;

import java.util.ArrayList;

import overlap.Overlap;
import overlap.Read;

//class for representing edges

public class Edge {

	private Vertex startVertex;
	private Vertex endVertex;
	private Read read;
	private Overlap overlap;
	private int beginIdx;
	private int endIdx;
	public boolean reduce;
	public final int type;
	private ArrayList<Edge> containedEdges=new ArrayList<>();
	
	public String toDNA(){
		//System.out.println(read);
		//System.out.println(overlap);
		String DNA=read.get(beginIdx, endIdx);
		for(Edge e:containedEdges){
			DNA+=e.toDNA();
		}
		return DNA;
	}
	
	public String toGFARecursive(){
		String GFA=toGFA();
		for(Edge e:containedEdges){
			GFA=GFA+"\n";
			GFA=GFA+e.toGFARecursive();
		}
		return GFA;
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
		return startVertex+"->"+endVertex+" TYPE:"+type+" LEN:"+length()+" CONT:"+containedEdges;//"Len:"+length();
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
	
	public int maxLength(){
		int max=segmentLength();
		for(Edge e:containedEdges){
			if(e.maxLength()>max){
				max=e.maxLength();
			}
		}
		return max;
	}

	public double minLo(){
		double min=1.0*overlap.size()/read.length();
		for(Edge e:containedEdges){
			if(e.minLo()<min){
				min=e.minLo();
			}
		}
		//System.out.println(min);
		return min;
	}

	public double maxLo(){
		double max=1.0*overlap.size()/read.length();
		for(Edge e:containedEdges){
			if(e.maxLo()>max){
				max=e.maxLo();
			}
		}
		//System.out.println(max);
		return max;
	}
	
	public int maxSegmentLength(){
		int max=segmentLength();
		for(Edge e:containedEdges){
			if(e.maxSegmentLength()>max){
				max=e.maxSegmentLength();
			}
		}
		return max;
	}
	
	public int minSegmentLength(){
		int min=segmentLength();
		for(Edge e:containedEdges){
			if(e.minSegmentLength()<min){
				min=e.minSegmentLength();
			}
		}
		return min;
	}



	public int segmentLength(){
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
	
	public Edge(Vertex startVertex,Vertex endVertex,Overlap overlap,Read read,int beginIdx,int endIdx,int type){
		this.startVertex=startVertex;
		this.endVertex=endVertex;
		this.read=read;
		this.overlap=overlap;
		this.beginIdx=beginIdx;
		this.endIdx=endIdx;
		this.type=type;
		startVertex.getOutEdges().add(this);
		endVertex.getInEdges().add(this);
		startVertex.sortEdges();
		//System.out.println(this);
	}

	public boolean hasInteriorVertices() {
		return containedEdges.size()>0;
	}
	
}
