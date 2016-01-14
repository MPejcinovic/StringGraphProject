
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

    std::string toDNA();
    Edge(Vertex *startVertex,Vertex *endVertex,Overlap *overlap,Read *read,int beginIdx,int endIdx,int type);
    ~Edge();
    
    /*  Returns the GFA representation of this edge
     */
    std::string toGFA();
    std::string toGFARecursive();
    
    std::string toString();
    
    /*  Returns overlaps from which this and all contained edges were created
     */
    std::vector<Overlap*> getOverlaps();
    
    /*  Returns the overlap from which this edge was created
     */
    Overlap* getOverlap();
    
    bool hasContainedEdges();
    
    int hashCode();
    
    Vertex* getStartVertex();
    
    /* Returns the read referenced by this edge (the edge's label is constructed using this read)
     */
    Read* getRead();
    
    Vertex* getEndVertex();
    
    /*  Returns the length of this edge (ignoring the contained edges)
     */
    int segmentLength();
    
    int numContainedEdges();
    
    
    /*  Returns the length of this edge (including the contained edges)
     */
    int length();
    
    /*  Removes this from the start and end vertices
     */
    void remove();
    
    /*  Adds edge to the list of contained edges, appends edge to this (reconnecting the end to edge->endVertex) and calles edge->remove()
     */
    void addEdge(Edge *edge);
    
    bool hasInteriorVertices();
};