package graph.operations;

import java.util.HashMap;
import java.util.Iterator;

import graph.Edge;
import graph.Graph;
import graph.Vertex;

public class NetworkFlowAnalyzer {

    private Graph graph;
    private double A=0;
    int NETWORK_FLOW_MIN_LENGTH=10000;
    
    boolean isEqualsOne(Edge edge){
        //std::cout <<"AA:"<<edge->length*A-edge->labelBegins.size()*log(2)<<"\n";
        return edge.length()*A-edge.numContainedEdges()*Math.log(2)>=17;
    }
    
    private void approximateAStatistic(){
        double G=0;
        double n=0;
        for(Vertex v:graph){
            for(Edge edge:v){
                if(A!=0&&!isEqualsOne(edge)){
                    continue;
                }
                if(edge.length()<NETWORK_FLOW_MIN_LENGTH){
                    continue;
                }
                G+=edge.length();
                n+=edge.numContainedEdges();
            }
        }
        A=n/G;
    }
    
   private enum Mark{
	   NETWORK_FLOW_MARK_EQUALS_1,
	   NETWORK_FLOW_MARK_MORE_OR_EQUAL_1,
	   NETWORK_FLOW_MARK_MORE_OR_EQUAL_0
   }
    
    private HashMap<Edge, Mark> edgeToMark=new HashMap<>();
    
    void processVertex(Vertex v){
        for(Edge edge:v){
            if(isEqualsOne(edge)){
            	edgeToMark.put(edge, Mark.NETWORK_FLOW_MARK_EQUALS_1);
                continue;
            }
            if(edge.hasInteriorVertices()){
            	edgeToMark.put(edge, Mark.NETWORK_FLOW_MARK_MORE_OR_EQUAL_1);
                continue;
            }
        	edgeToMark.put(edge, Mark.NETWORK_FLOW_MARK_MORE_OR_EQUAL_0);
        }
    }
    
    private HashMap<Vertex, Integer> B=new HashMap<>();
    
    void calculateB(Vertex v){
        int sum=0;
        for(Edge edge:v.getInEdges()){
            if(edgeToMark.get(edge)!=Mark.NETWORK_FLOW_MARK_MORE_OR_EQUAL_0){
                sum++;
            }
        }
        for(Edge edge:v){
            if(edgeToMark.get(edge)!=Mark.NETWORK_FLOW_MARK_MORE_OR_EQUAL_0){
                sum--;
            }
        }
        B.put(v, sum);
    }
    
    void removeEdges(Vertex v){
    	Iterator<Edge> iterator=v.iterator();
        while(iterator.hasNext()){
            Edge vw=iterator.next();
            Vertex w=vw.getEndVertex();
            if(edgeToMark.get(vw)==Mark.NETWORK_FLOW_MARK_EQUALS_1){
            	iterator.remove();
            	w.getInEdges().remove(vw);
            }
        }
    }
    
    void removeVertices(){
    	Iterator<Vertex> iterator=graph.iterator();
        while(iterator.hasNext()) {
        	Vertex v=iterator.next();
            if(B.get(v)!=0){
                continue;
            }
            if(v.getInEdges().size()==0||v.getOutEdges().size()==0){
            	Iterator<Edge> it=v.getInEdges().iterator();
            	while(it.hasNext()){
            		Edge e=it.next();
            		e.getStartVertex().getOutEdges().remove(e);
            		it.remove();
            	}
            	it=v.getOutEdges().iterator();
            	while(it.hasNext()){
            		Edge e=it.next();
            		e.getEndVertex().getInEdges().remove(e);
            		it.remove();
            	}
            	iterator.remove();
            }
        }
    }
        
    public void process(){
    	approximateAStatistic();
		System.out.println(A);

    	for(Vertex v:graph){
            processVertex(v);
        }
        for(Vertex v:graph){
        	calculateB(v);
        }
        for(Vertex v:graph){
        	removeEdges(v);
        }

        removeVertices();
    }
    
    public NetworkFlowAnalyzer(Graph graph){
        this.graph=graph;
    }
    

	
}
