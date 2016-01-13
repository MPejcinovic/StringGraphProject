

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
    //private HashMap<Edge, Boolean> edgeToReduce;
    
    int mark(Vertex *v){
        return vertexMarks[v];
    }
    
    void mark(Vertex *v,int m){
        vertexMarks[v]=m;
    }
    
    void removeTransitiveEdges(Vertex *v,int maxLen){
        v->sortEdges();
        for(Edge *vw:v->getOutEdges()){
            Vertex *w=vw->getEndVertex();
            w->sortEdges();
            if(mark(w)!=INPLAY){
                continue;
            }
            
            for(Edge *wx:w->getOutEdges()){
                if(vw->length()+wx->length()>maxLen){
                    break;
                }
                Vertex *x=wx->getEndVertex();
                if(mark(x)==INPLAY){
                    count1++;
                    mark(x,ELIMINATED);
                }
            }
        }
    }
    
    void removeShortEdges(Vertex *v){
        for(Edge *vw :v->getOutEdges()){
            Vertex *w=vw->getEndVertex();
            int numIter=0;
            //w.sortEdges();
            for(Edge *wx : w->getOutEdges()){
                Vertex *x=wx->getEndVertex();
                numIter++;
                if(mark(x)!=INPLAY){
                    continue;
                }
                if(numIter==1){
                    count2++;
                    mark(x,ELIMINATED);
                }
                else if(wx->length()<FUZZ){
                    count2++;
                    mark(x,ELIMINATED);
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
            mark(w, INPLAY);
        }
    }
    
    void markEliminatedEdgesAndReset(Vertex *v){
        for(Edge *vw :v->getOutEdges()){
            Vertex *w=vw->getEndVertex();
            if(mark(w)==ELIMINATED){
                vw->reduce=true;
                //edgeToReduce.put(vw, true);
            }
            mark(w,VACANT);
        }
    }
    
    
    void processVertex(Vertex *v){
        prepareVertex(v);
        int maxLen=200000000;
        if(v->getOutEdges().size()!=0){
            maxLen=v->getOutEdges()[v->getOutEdges().size()-1]->length()+FUZZ;
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
                /*if(!edgeToReduce.get(e)){
                 continue;
                 }*/
                if(!e->reduce){
                    ++it;
                    continue;
                }
                //System.out.println("REMOVED EDGE:"+e);
                it=v->getOutEdges().erase(it);
                e->getEndVertex()->getInEdges().erase(std::find(e->getEndVertex()->getInEdges().begin(), e->getEndVertex()->getInEdges().end(), e));
            }
        }
    }
    
public:
    void process(){
        //edgeToReduce=new HashMap<>();
        for(Vertex *v:graph->vertices){
            vertexMarks[v]=VACANT;
            for(Edge *e:v->getOutEdges()){
                /*if(edgeToReduce.get(e)!=null){
                 System.out.println("AAA");
                 }*/
                e->reduce=false;
                //edgeToReduce.put(e, false);
            }
        }
        for(Vertex *v:graph->vertices){
            processVertex(v);
        }
        std::cout <<"Transitive removed:" <<count1 << "\n";
        std::cout <<"Short removed:" <<count2 << "\n";
        removeReducedEdges();
    }
    
    TransitiveEdgeRemover(Graph *graph){
        this->graph=graph;
    }
};