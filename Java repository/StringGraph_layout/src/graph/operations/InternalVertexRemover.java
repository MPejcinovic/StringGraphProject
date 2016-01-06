package graph.operations;

import graph.Edge;
import graph.Graph;
import graph.Vertex;

public class InternalVertexRemover {

	private Graph graph;
	
	private void processVertex(Vertex v){
		for(Edge vw:v){
			Vertex w=vw.getEndVertex();
			do{
				if(w.getInEdges().size()!=1||w.getOutEdges().size()!=1){
					break;
				}
				Edge wx=w.getOutEdges().get(0);
				vw.addEdge(wx);
				w=wx.getEndVertex();
			}while(true);
		}
	}
	
	public void process(){
		for(Vertex v:graph){
			processVertex(v);
		}
	}
	
	public InternalVertexRemover(Graph graph) {
		this.graph=graph;
	}
	
}
