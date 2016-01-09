package graph.operations;

import java.util.HashMap;
import java.util.Iterator;

import graph.Edge;
import graph.Graph;
import graph.Vertex;

public class TransitiveEdgeRemover {

	private static int FUZZ=10;
	
	private enum Mark{
		INPLAY,
		VACANT,
		ELIMINATED
	}
	
	int count1=0;
	int count2=0;
	private Graph graph;
	private HashMap<Vertex, Mark> vertexMarks;
	//private HashMap<Edge, Boolean> edgeToReduce;
	
	private Mark mark(Vertex v){
		return vertexMarks.get(v);
	}

	private void mark(Vertex v,Mark m){
		vertexMarks.put(v, m);
	}

	private void removeTransitiveEdges(Vertex v,int maxLen){
		v.sortEdges();
		for(Edge vw:v){
			Vertex w=vw.getEndVertex();
			w.sortEdges();
			if(mark(w)!=Mark.INPLAY){
				continue;
			}
						
			for(Edge wx:w){
				if(vw.length()+wx.length()>maxLen){
					break;
				}
				Vertex x=wx.getEndVertex();
				if(mark(x)==Mark.INPLAY){
					count1++;
					mark(x,Mark.ELIMINATED);
				}
			}
		}
	}

	private void removeTransitiveEdges2(Vertex v,int maxLen){
		v.sortEdges();
		for(Edge vw:v){
			Vertex w=vw.getEndVertex();
			w.sortEdges();
			if(mark(w)==Mark.INPLAY){
				for(Edge wx:w){
					if(vw.length()+wx.length()<=maxLen){
						Vertex x=wx.getEndVertex();
						if(mark(x)==Mark.INPLAY){
							count1++;
							mark(x,Mark.ELIMINATED);
						}
					}
				}
			}
						
		}
	}

	private void removeShortEdges(Vertex v){
        for(Edge vw :v){
            Vertex w=vw.getEndVertex();
            int numIter=0;
            //w.sortEdges();
            for(Edge wx : w){
                Vertex x=wx.getEndVertex();
                numIter++;
                if(mark(x)!=Mark.INPLAY){
                    continue;
                }
                if(numIter==1){
                	count2++;
                	mark(x,Mark.ELIMINATED);
                }
                else if(wx.length()<FUZZ){
                	count2++;
                	mark(x,Mark.ELIMINATED);
                }
                else if (wx.length()>=FUZZ){
                    break;
                }
            }
        }
	}

	private void removeShortEdges2(Vertex v){
        for(Edge vw :v){
        	Vertex w=vw.getEndVertex();
        	int widx=0;
        	for(Edge wx:w){
        		widx++;
        		if(wx.length()<FUZZ||widx==1){
        			if(mark(wx.getEndVertex())==Mark.INPLAY){
        				mark(wx.getEndVertex(), Mark.ELIMINATED);
        			}
        		}
        	}
        }
	}

	
    private void prepareVertex(Vertex v){
    	v.sortEdges();
        for(Edge vw:v){
            Vertex w=vw.getEndVertex();
            mark(w, Mark.INPLAY);
        }
    }

    private void markEliminatedEdgesAndReset(Vertex v){
        for(Edge vw :v){
            Vertex w=vw.getEndVertex();
            if(mark(w)==Mark.ELIMINATED){
            	vw.reduce=true;
            	//edgeToReduce.put(vw, true);
            }
            mark(w,Mark.VACANT);
        }
    }

	
	private void processVertex(Vertex v){
		prepareVertex(v);
		int maxLen=200000000;
		if(v.getOutEdges().size()!=0){
			maxLen=v.getOutEdges().get(v.getOutEdges().size()-1).length()+FUZZ;
		}
		removeTransitiveEdges(v, maxLen);
		removeShortEdges(v);
		markEliminatedEdgesAndReset(v);
	}
	
    void removeReducedEdges(){
    	for(Vertex v:graph){
    		Iterator<Edge> it=v.iterator();
    		while(it.hasNext()){
    			Edge e=it.next();
    			/*if(!edgeToReduce.get(e)){
    				continue;
    			}*/
    			if(!e.reduce){
    				continue;
    			}
    			//System.out.println("REMOVED EDGE:"+e);
    			it.remove();
    			e.getEndVertex().getInEdges().remove(e);
    		}
    	}
    }
	
	public void process(){
		vertexMarks=new HashMap<>();
		//edgeToReduce=new HashMap<>();
		for(Vertex v:graph){
			vertexMarks.put(v, Mark.VACANT);
			for(Edge e:v){
				/*if(edgeToReduce.get(e)!=null){
					System.out.println("AAA");
				}*/
				e.reduce=false;
				//edgeToReduce.put(e, false);
			}
		}
		for(Vertex v:graph){
			processVertex(v);
		}
		System.out.println("Transitive removed:"+count1);
		System.out.println("Short removed:"+count2);
		removeReducedEdges();
	}
	
	public TransitiveEdgeRemover(Graph graph){
		this.graph=graph;
	}
	
}
