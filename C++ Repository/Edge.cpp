

#include "Edge.hpp"
#include "Vertex.hpp"
#include <sstream>
#include "DebugPrint.hpp"

std::string Edge::toString(){
    std::stringstream ss;
    ss << startVertex->id;
    for (auto i: labelIDs)
        ss<<"->" << i;
    ss<< "\n";
    return ss.str();
}

void Edge::addEdge(Edge *edge){
    labelIDs.insert(labelIDs.end(), edge->labelIDs.begin(), edge->labelIDs.end());
    labelBegins.insert(labelBegins.end(), edge->labelBegins.begin(), edge->labelBegins.end());
    labelEnds.insert(labelEnds.end(), edge->labelEnds.begin(), edge->labelEnds.end());
    length+=edge->length;
    std::remove(endVertex->inEdges.begin(), endVertex->inEdges.end(), this);
    endVertex=edge->endVertex;
    edge->endVertex->inEdges.push_back(this);
    startVertex->sortEdges();
    //endVertex->sortEdges();
}

void Edge::addToVertices(){
    startVertex->edges.push_back(this);
    endVertex->inEdges.push_back(this);
}

void Edge::remove(){
    DebugPrint::print("Edge::remove()\n");
    DebugPrint::print(toString());
    DebugPrint::print(startVertex->toString());
    DebugPrint::print(endVertex->toString());
    startVertex->edges.erase(std::remove(startVertex->edges.begin(), startVertex->edges.end(), this));
    endVertex->inEdges.erase(std::remove(endVertex->inEdges.begin(), endVertex->inEdges.end(), this));
}
