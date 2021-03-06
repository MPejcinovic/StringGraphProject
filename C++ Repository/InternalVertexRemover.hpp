

#pragma once
#include "Graph.hpp"
#include <iostream>
#include "DebugPrint.hpp"

#define INTERNAL_MARK_VACANT 0
#define INTERNAL_MARK_ELIMINATED 1

class InternalVertexRemover {
private:
    Graph *graph;
    
    
    void processVertex(Vertex *v){
        for(Edge *vw:v->getOutEdges()){
            Vertex *w=vw->getEndVertex();
            do{
                if(w->getInEdges().size()!=1||w->getOutEdges().size()!=1){ //if in or out degree is different than zero stop
                    break;
                }
                Edge *wx=w->getOutEdges()[0]; //vertex has in and out degree equal to one, absorb it's out edge
                vw->addEdge(wx);
                w=wx->getEndVertex();
            }while(true);
        }
    }
    
public:
    void process(){
        for(Vertex *v:graph->vertices){
            processVertex(v);
        }
    }
    
    InternalVertexRemover(Graph *graph) {
        this->graph=graph;
    }
    
};