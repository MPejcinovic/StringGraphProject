
#pragma once
#include <vector>
#include <iostream>

class Vertex;

class Edge {
    
public:
    
    void addEdge(Edge *edge);
    
    void remove();
    
    std::string toString();
    void addToVertices();
    bool hasInteriorVertices(){
        return labelIDs.size()>1;
    }
    
    Edge(Vertex* startVertex,Vertex* endVertex,int labelID,int labelBegin,int labelEnd,int overlapIDX):endVertex(endVertex),startVertex(startVertex){
        this->overlapIDX=overlapIDX;
        labelIDs.push_back(labelID);
        labelBegins.push_back(labelBegin);
        labelEnds.push_back(labelEnd);
        length=labelBegin-labelEnd;
        if(length<0){
            length=-length;
        }
        addToVertices();
    }
    
    Vertex *startVertex;
    Vertex *endVertex;
    int overlapIDX;
    std::vector<int> labelIDs;
    std::vector<int> labelBegins;
    std::vector<int> labelEnds;
    int length;
    int mark;
    
    
    
};