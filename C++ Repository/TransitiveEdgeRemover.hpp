

#pragma once
#include "Graph.hpp"
#include <map>

#define VACANT 0
#define INPLAY 1
#define ELIMINATED 2

#define NO_REDUCE 0
#define REDUCE 1
#define FUZZ 10

class TransitiveEdgeRemover {
    
private:
    
    int count1=0;
    int count2=0;
    Graph *graph;
    std::map<Vertex*, int> vertexMarks;
    std::map<Edge*,bool> edgeToReduce;
    
    int getMark(Vertex *v){
        return vertexMarks[v];
    }
    
    void setMark(Vertex *v,int m){
        vertexMarks[v]=m;
    }
    
    void removeTransitiveEdges(Vertex *v,int maxLen){
        v->sortEdges();
        for(Edge *vw:v->getOutEdges()){
            Vertex *w=vw->getEndVertex();
            w->sortEdges();
            if(getMark(w)!=INPLAY){
                continue;
            }
            
            for(Edge *wx:w->getOutEdges()){
                if(vw->length()+wx->length()>maxLen){
                    break;
                }
                Vertex *x=wx->getEndVertex();
                if(getMark(x)==INPLAY){
                    count1++;
                    setMark(x,ELIMINATED);
                }
            }
        }
    }
    
    void removeShortEdges(Vertex *v){
        for(Edge *vw :v->getOutEdges()){
            Vertex *w=vw->getEndVertex();
            int numIter=0;
            for(Edge *wx : w->getOutEdges()){
                Vertex *x=wx->getEndVertex();
                numIter++;
                if(getMark(x)!=INPLAY){
                    continue;
                }
                if(numIter==1){
                    count2++;
                    setMark(x,ELIMINATED);
                }
                else if(wx->length()<FUZZ){
                    count2++;
                    setMark(x,ELIMINATED);
                }
                else if (wx->length()>=FUZZ){
                    break;
                }
            }
        }
    }
    
    void prepareVertex(Vertex *v){
        v->sortEdges();
        for(Edge *vw:v->getOutEdges()){
            Vertex *w=vw->getEndVertex();
            setMark(w, INPLAY);
        }
    }
    
    void markEliminatedEdgesAndReset(Vertex *v){
        for(Edge *vw :v->getOutEdges()){
            Vertex *w=vw->getEndVertex();
            if(getMark(w)==ELIMINATED){
                edgeToReduce[vw]=true;
                //edgeToReduce.put(vw, true);
            }
            setMark(w,VACANT);
        }
    }
    
    
    void processVertex(Vertex *v){
        prepareVertex(v);
        int maxLen=200000000;
        if(v->getOutEdges().size()!=0){
            maxLen=v->getOutEdges()[v->getOutEdges().size()-1]->length()+FUZZ;  //take the longest edge+FUZZ
        }
        removeTransitiveEdges(v, maxLen);
        removeShortEdges(v);
        markEliminatedEdgesAndReset(v);
    }
    
    void removeReducedEdges(){
        for(Vertex *v:graph->vertices){
            auto it=v->getOutEdges().begin();
            while(it!=v->getOutEdges().end()){
                Edge *e=*it;
                if(!edgeToReduce[e]){//skip if shouldn't reduce
                    ++it;
                    continue;
                }
                it=v->getOutEdges().erase(it);
                e->getEndVertex()->getInEdges().erase(std::find(e->getEndVertex()->getInEdges().begin(), e->getEndVertex()->getInEdges().end(), e));
                delete e;
            }
        }
    }
    
public:

    void process(){
        for(Vertex *v:graph->vertices){
            vertexMarks[v]=VACANT;      //mark all vertices as vacant
            for(Edge *e:v->getOutEdges()){
                edgeToReduce[e]=false;  //mark all edges as don't_reduce
            }
        }
        for(Vertex *v:graph->vertices){
            processVertex(v);//process all vertices
        }
        std::cout <<"Transitive removed:" <<count1 << "\n";
        std::cout <<"Short removed:" <<count2 << "\n";
        removeReducedEdges();
    }
    
    TransitiveEdgeRemover(Graph *graph){
        this->graph=graph;
    }
};