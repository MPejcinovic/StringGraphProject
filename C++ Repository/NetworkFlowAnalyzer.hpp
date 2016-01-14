
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
    
    /*  Calculates the A-statistic on the graph
     */
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
    
    /*  Mark the edges of v using the calculated A-statistic
     */
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
    
    /*  Calculates the demand of flow through v
     */
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
    
    /*  Remove every edge that has the upper bound of flow 0
     */

    void removeEdges(Vertex *v){
        auto iterator=v->getOutEdges().begin();
        while(iterator!=v->getOutEdges().end()){
            Edge *vw=*iterator;
            Vertex *w=vw->getEndVertex();
            if(edgeToMark[vw]==NETWORK_FLOW_MARK_EQUALS_1){
               iterator=v->getOutEdges().erase(iterator);
                w->getInEdges().erase(std::find(w->getInEdges().begin(), w->getInEdges().end(), vw));
                delete vw;
            }else{
                ++iterator;
            }
        }
    }
    
    /*  Remove each vertex with supply of flow equal to 0 and out-degree or in-degree equal to 0
     */
    void removeVertices(){
        auto iterator=graph->vertices.begin();
        while(iterator!=graph->vertices.end()) {
            Vertex *v=*iterator;
            if(B[v]!=0){
                ++iterator;
                continue;
            }
            if(v->getInEdges().size()==0||v->getOutEdges().size()==0){
                iterator=graph->vertices.erase(iterator);
                delete v;
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