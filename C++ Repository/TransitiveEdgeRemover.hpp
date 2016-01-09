

#pragma once
#include "Graph.hpp"
#include "DebugPrint.hpp"

#define MARK_VACANT 0
#define MARK_INPLAY 1
#define MARK_ELIMINATED 2

#define NO_REDUCE 0
#define REDUCE 1


class TransitiveEdgeRemover {
    
private:
    Graph& graph;
    int count1=0;
    int count2=0;
    
public:
    static int FUZZ;
        
    void prepareData(){
        for(auto& entry:graph.idToVertexMap){
            entry.second->mark=MARK_VACANT;
            for(Edge *edge :entry.second->edges){
                edge->mark=NO_REDUCE;
            }
        }
    }
    
    int prepareVertex(Vertex *V){
        int maxLen=0;
        for(Edge *VW :V->edges){
            Vertex *W=VW->endVertex;
            W->mark=MARK_INPLAY;
            if(VW->length>maxLen){
                maxLen=VW->length;
            }
        }
        return maxLen;
    }
    
    void removeTransitiveEdges(Vertex *V,int maxLen){
        for(Edge *VW :V->edges){
            Vertex *W=VW->endVertex;
            if(W->mark!=MARK_INPLAY){
                continue;
            }
            for(Edge *WX : W->edges){
                if(VW->length+WX->length>maxLen){
                    break;
                }
                Vertex *X=WX->endVertex;
                if(X->mark==MARK_INPLAY){
                    DebugPrint::print("Marking vertex(removeTransitiveEdges) V:"+std::to_string(V->id) +"X:"+std::to_string(X->id)+"\n");
                    count1++;
                    X->mark=MARK_ELIMINATED;
                }
            }
        }
    }
    
    void removeShortEdges(Vertex *V){
        for(Edge *VW :V->edges){
            Vertex *W=VW->endVertex;
            DebugPrint::print(V->toString());
            DebugPrint::print(W->toString());
            int numIter=0;
            for(Edge *WX : W->edges){
                Vertex *X=WX->endVertex;
                numIter++;
                if(X->mark!=MARK_INPLAY){
                    DebugPrint::print("Skipping vertex:"+std::to_string(X->id)+"\n");
                    continue;
                }
                if(numIter==1){
                    DebugPrint::print("Marking vertex(removeShortEdges numIter=1) V:"+std::to_string(V->id)+" X:"+std::to_string(X->id)+"\n");
                    count2++;
                    X->mark=MARK_ELIMINATED;
                }
                else if(WX->length<FUZZ){
                    DebugPrint::print("Marking vertex(removeShortEdges fuzz) V:"+std::to_string(V->id)+" X:"+std::to_string(X->id)+"\n");
                    count2++;
                    X->mark=MARK_ELIMINATED;
                }
                else if (WX->length>=FUZZ){
                    break;
                }
            }
        }
    }
    
    void markEliminatedEdgesAndReset(Vertex *V){
        for(Edge *VW :V->edges){
            Vertex *W=VW->endVertex;
            if(W->mark==MARK_ELIMINATED){
                VW->mark=REDUCE;
            }
            W->mark=MARK_VACANT;
        }
    }
    
    void processVertex(Vertex *V){
        int maxLen=prepareVertex(V);
        maxLen+=FUZZ;
        removeTransitiveEdges(V, maxLen);
        removeShortEdges(V);
        markEliminatedEdgesAndReset(V);
    }
    
    void removeReducedEdges(){
        for(auto& entry:graph.idToVertexMap){
            Vertex *V=entry.second;
            auto iterator=V->edges.begin();
            while(iterator!=V->edges.end()){
                Edge *VW=*iterator;
                Vertex *W=VW->endVertex;
                if(VW->mark==REDUCE){
                    DebugPrint::print("Removing edge:");
                    DebugPrint::print(VW->toString());
                    iterator=V->edges.erase(iterator);
                    W->inEdges.erase(std::remove(W->inEdges.begin(), W->inEdges.end(), VW));
                    delete VW;
                }else{
                    ++iterator;
                }
            }
        }
    }
    
    void process(){
        DebugPrint::print(graph.toString());
        prepareData();
        for(auto& entry:graph.idToVertexMap){
            processVertex(entry.second);
        }
        std::cout <<"Count1:"<<count1<<"\n";
        std::cout <<"Count2:"<<count2<<"\n";
        removeReducedEdges();
    }
    
    TransitiveEdgeRemover(Graph&graph):graph(graph){
        
    }
};