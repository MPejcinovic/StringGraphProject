

#include "Edge.hpp"
#include "Vertex.hpp"
#include <sstream>
#include "DebugPrint.hpp"


Edge::Edge(Vertex *startVertex,Vertex *endVertex,Overlap *overlap,Read *read,int beginIdx,int endIdx,int type){
    this->startVertex=startVertex;
    this->endVertex=endVertex;
    this->read=read;
    read->REFERENCE++;
    this->overlap=overlap;
    this->beginIdx=beginIdx;
    this->endIdx=endIdx;
    this->type=type;
    startVertex->getOutEdges().push_back(this);
    endVertex->getInEdges().push_back(this);
    startVertex->sortEdges();
}

std::string Edge::toDNA(){
    std::string DNA=read->get(beginIdx, endIdx);
    for(Edge *e:containedEdges){
        DNA+=e->toDNA();
    }
    return DNA;
}

std::string Edge::toGFA(){
    std::stringstream ss;
    ss << "L\t" <<
				overlap->leftPart()->getRead()->getID() <<
				"\t" << (overlap->leftPart()->isForward()?"+":"-") << "\t" <<
				overlap->rightPart()->getRead()->getID() <<
				"\t" << (overlap->rightPart()->isForward()?"+":"-") << "\t" <<
				overlap->size() << "M";
    return ss.str();
}

std::string Edge::toGFARecursive(){
    std::string GFA=toGFA();
    for(Edge *e:containedEdges){
        GFA.append("\n");
        GFA.append(e->toGFARecursive());
    }
    return GFA;
}


std::string Edge::toString() {
    std::stringstream ss;
    ss << startVertex->toString() << "->" << endVertex->toString() << " TYPE:" << type << " LEN:" << length() << " CONT:"<< containedEdges.size();
    return ss.str();
}

std::vector<Overlap*> Edge::getOverlaps(){
    std::vector<Overlap*> overlaps;
    overlaps.push_back(getOverlap());
    for(Edge *edge:containedEdges){
        std::vector<Overlap*> eo=edge->getOverlaps();
        overlaps.insert(overlaps.begin(),eo.begin(),eo.end());
    }
    return overlaps;
}

Overlap* Edge::getOverlap(){
    return overlap;
}

bool Edge::hasContainedEdges(){
    return containedEdges.size()>0;
}

int Edge::hashCode() {
    return 31*startVertex->hashCode()+37*endVertex->hashCode();
}

Vertex* Edge::getStartVertex(){
    return startVertex;
}

Read* Edge::getRead(){
    return read;
}

Vertex* Edge::getEndVertex(){
    return endVertex;
}

int Edge::segmentLength(){
    int l=beginIdx-endIdx;
    if(l<0){
        l=-l;
    }
    return l;
}

int Edge::numContainedEdges(){
    int num=0;
    for(Edge *edge:containedEdges){
        num+=1;
        num+=edge->numContainedEdges();
    }
    return num;
}

Edge::~Edge(){
    for(Edge *edge:containedEdges){
        delete edge;
    }
    overlap->REFERENCE--;
    if(overlap->REFERENCE==0){
        delete overlap;   
    }
    read->REFERENCE--;
    if(read->REFERENCE==0){
        delete read;
    }
}

int Edge::length(){
    int length=segmentLength();
    for(Edge *edge:containedEdges){
        length+=edge->length();
    }
    return length;
}

void Edge::remove(){
    startVertex->getOutEdges().erase(std::find(startVertex->getOutEdges().begin(), startVertex->getOutEdges().end(), this));
    endVertex->getInEdges().erase(std::find(endVertex->getInEdges().begin(), endVertex->getInEdges().end(), this));
}

void Edge::addEdge(Edge *edge){
    containedEdges.push_back(edge);
    endVertex->getInEdges().erase(std::find(endVertex->getInEdges().begin(), endVertex->getInEdges().end(), this));
    endVertex=edge->getEndVertex();
    endVertex->getInEdges().push_back(this);
    edge->remove();
}


bool Edge::hasInteriorVertices() {
    return containedEdges.size()>0;
}
