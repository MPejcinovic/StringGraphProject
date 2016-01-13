
#pragma once
#include <vector>
#include <iostream>
#include "Read.hpp"
#include "Overlap.hpp"

class Vertex;

class Edge {
    
private:
    Vertex *startVertex;
    Vertex *endVertex;
    Read *read;
    Overlap *overlap;
    int beginIdx;
    int endIdx;
    int type;
    std::vector<Edge*> containedEdges;
    
public:
    bool reduce;

    std::string toDNA();
    Edge(Vertex *startVertex,Vertex *endVertex,Overlap *overlap,Read *read,int beginIdx,int endIdx,int type);
    
    std::string toGFA();
    
    std::string toString();
    
    std::vector<Overlap*> getOverlaps();
    
    Overlap* getOverlap();
    
    bool hasContainedEdges();
    
    int hashCode();
    
    Vertex* getStartVertex();
    
    Read* getRead();
    
    Vertex* getEndVertex();
    
    int segmentLength();
    
    int numContainedEdges();
    
    int length();
    
    void remove();
    
    void addEdge(Edge *edge);
    
    bool hasInteriorVertices();
};