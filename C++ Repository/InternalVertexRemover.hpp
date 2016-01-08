

#pragma once
#include "Graph.hpp"
#include <iostream>
#include "DebugPrint.hpp"

#define INTERNAL_MARK_VACANT 0
#define INTERNAL_MARK_ELIMINATED 1

class InternalVertexRemover {
    
private:
    Graph& graph;
    
public:
    
    void prepareVertices(){
        for(auto& entry:graph.idToVertexMap){
            Vertex *V=entry.second;
            V->mark=INTERNAL_MARK_VACANT;
            if(V->edges.size()==1&&V->edges[0]->endVertex==V){
                DebugPrint::print("A\n");
            }
        }
    }
    
    
    
    void processVertex(Vertex *V){
        DebugPrint::print("Processing vertex:" +std::to_string(V->id) +"\n");
        if(V->edges.size()==0){
            DebugPrint::print("Empty vertex:" +std::to_string(V->id) +"\n");
            return;
        }
        Edge *VW=V->edges[0];
        DebugPrint::print("Only edge:");
        DebugPrint::print(VW->toString());

        while(true){
            Vertex *W=VW->endVertex;
            DebugPrint::print(W->toString());
            if(W->edges.size()!=1){
                return;
            }
            if(W->inEdges.size()!=1){
                return;
            }
            
            /*if(V->isBegin){
                std::cout <<"V is begin ";
            }else{
                std::cout <<"V is end ";
            }
            
            if(W->isBegin){
                std::cout <<"W is begin\n";
            }else{
                std::cout <<"W is end\n";
            }*/
            
            DebugPrint::print("Edge vertices: " +std::to_string( V->id));

            DebugPrint::print("Marking vertex "+std::to_string(W->id) +" as eliminated!\n");
            W->mark=INTERNAL_MARK_ELIMINATED;
            Edge *WX=W->edges[0];
            Vertex *X=WX->endVertex;
            DebugPrint::print("X:"+std::to_string(X->id));
            VW->addEdge(WX);
            WX->remove();
        }
    }
    
    void deleteVertices(){
        auto iterator=graph.idToVertexMap.begin();
        while(iterator!=graph.idToVertexMap.end()){
            Vertex *V=(*iterator).second;
            if(V->mark==INTERNAL_MARK_ELIMINATED){
                DebugPrint::print("Deleting vertex "+std::to_string(V->id)+" reversed:" +std::to_string(V->isBegin)+"\n");
                iterator=graph.idToVertexMap.erase(iterator);
                delete V;
            }else{
                ++iterator;
            }
        }
    }
    
    void process(){
        DebugPrint::print("Prep\n");
        DebugPrint::print(graph.toString());
        prepareVertices();
        DebugPrint::print("After Prep\n");
        for(auto& entry:graph.idToVertexMap){
            Vertex *V=entry.second;
            processVertex(V);
        }
        DebugPrint::print("DEL\n");
        deleteVertices();
    }
    
    InternalVertexRemover(Graph& graph):graph(graph){
    }
    
    
};