
#pragma once

#include "Graph.hpp"
#include "DebugPrint.hpp"
#include "Vertex.hpp"
#include <map>
#include <math.h>
#include "Edge.hpp"
#include "Vertex.hpp"
#include <iterator>

#define NETWORK_FLOW_MARK_EQUALS_1 0
#define NETWORK_FLOW_MARK_MORE_OR_EQUAL_0 1
#define NETWORK_FLOW_MARK_MORE_OR_EQUAL_1 2
#define NETWORK_FLOW_MIN_LENGTH 10000

class NetworkFlowAnalyzer {
    
private:
    Graph *graph;
    double A=0;
    
    bool isEqualsOne(Edge *edge){
        return edge->length()*A-edge->numContainedEdges()*log(2)>=17;
    }
    
    void approximateAStatistic(){
        double G=0;
        double n=0;
        for(Vertex *v:graph->vertices){
            for(Edge *edge:v->getOutEdges()){
                if(A!=0&&!isEqualsOne(edge)){
                    continue;
                }
                if(edge->length()<NETWORK_FLOW_MIN_LENGTH){
                    continue;
                }
                G+=edge->length();
                n+=edge->numContainedEdges();
            }
        }
        A=n/G;
    }
    
    std::map<Edge*, int> edgeToMark;
    
    void processVertex(Vertex *v){
        for(Edge *edge:v->getOutEdges()){
            if(isEqualsOne(edge)){
                edgeToMark[edge]=NETWORK_FLOW_MARK_EQUALS_1;
                continue;
            }
            if(edge->hasInteriorVertices()){
                edgeToMark[edge]=NETWORK_FLOW_MARK_MORE_OR_EQUAL_1;
                continue;
            }
            edgeToMark[edge]=NETWORK_FLOW_MARK_MORE_OR_EQUAL_0;
        }
    }
    
    std::map<Vertex*, int> B;
    
    void calculateB(Vertex *v){
        int sum=0;
        for(Edge *edge:v->getInEdges()){
            if(edgeToMark.find(edge)->second!=NETWORK_FLOW_MARK_MORE_OR_EQUAL_0){
                sum++;
            }
        }
        for(Edge *edge:v->getOutEdges()){
            if(edgeToMark.find(edge)->second!=NETWORK_FLOW_MARK_MORE_OR_EQUAL_0){
                sum--;
            }
        }
        B[v]=sum;
    }
    
    void removeEdges(Vertex *v){
        auto iterator=v->getOutEdges().begin();
        while(iterator!=v->getOutEdges().end()){
            Edge *vw=*iterator;
            Vertex *w=vw->getEndVertex();
            if(edgeToMark[vw]==NETWORK_FLOW_MARK_EQUALS_1){
               iterator=v->getOutEdges().erase(iterator);
                w->getInEdges().erase(std::find(w->getInEdges().begin(), w->getInEdges().end(), vw));
            }else{
                ++iterator;
            }
        }
    }
    
    void removeVertices(){
        auto iterator=graph->vertices.begin();
        while(iterator!=graph->vertices.end()) {
            Vertex *v=*iterator;
            if(B[v]!=0){
                ++iterator;
                continue;
            }
            if(v->getInEdges().size()==0||v->getOutEdges().size()==0){
                auto it=v->getInEdges().begin();
                while(it!=v->getInEdges().end()){
                    Edge *e=*it;
                    e->getStartVertex()->getOutEdges().erase(std::find(e->getStartVertex()->getOutEdges().begin(), e->getStartVertex()->getOutEdges().end(), e));
                    it=v->getInEdges().erase(it);
                }
                it=v->getOutEdges().begin();
                while(it!=v->getOutEdges().end()){
                    Edge *e=*it;
                    e->getEndVertex()->getInEdges().erase(std::find(e->getEndVertex()->getInEdges().begin(), e->getEndVertex()->getInEdges().end(), e));
                    it=v->getOutEdges().erase(it);
                }
                iterator=graph->vertices.erase(iterator);
            }
            else{
                ++iterator;
            }
        }
    }
    
public:
    
    void process(){
        approximateAStatistic();
        for(Vertex *v:graph->vertices){
            processVertex(v);
        }
        for(Vertex *v:graph->vertices){
            calculateB(v);
        }
        for(Vertex *v:graph->vertices){
            removeEdges(v);
        }
        removeVertices();
    }
    
    NetworkFlowAnalyzer(Graph *graph){
        this->graph=graph;
    }
    

    
};