

#pragma once
#include <vector>
#include "Edge.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include "Read.hpp"

class Vertex {
    
private:
    Read *read;
    std::vector<Edge*> outEdges;
    std::vector<Edge*> inEdges;

public:
    bool BEGIN;
    
    ~Vertex(){
        for(Edge *edge:inEdges){
            auto find=std::find(edge->getStartVertex()->outEdges.begin(),edge->getStartVertex()->outEdges.end(),edge);
            edge->getStartVertex()->outEdges.erase(find);
            delete edge;
        }
        for(Edge *edge:outEdges){
            auto find=std::find(edge->getEndVertex()->inEdges.begin(),edge->getEndVertex()->inEdges.end(),edge);
            edge->getEndVertex()->inEdges.erase(find);
            delete edge;
        }
        read->REFERENCE--;
        if(read->REFERENCE==0){
            //std::cout << read->REFERENCE<< " REF: " << read->ID << "\n";
            delete read;
        }

    }
    
    
    /*  Returns the GFA representation of the read
     */
    std::string toGFA(){
        std::stringstream ss;
        ss << "S " << read->getID() << " " << read->sequence;
        return ss.str();
    }
    
    
    std::string toString() {
        std::stringstream ss;
        ss << read->getID() << (BEGIN?".B":".E");
        return ss.str();
    }
    
    Read* getRead(){
        return read;
    }
    
    static int hash(Read *read,bool BEGIN){
        return read->hashCode()*(BEGIN?1:-1);
    }
    
    int hashCode() {
        return hash(getRead(), BEGIN);
    }
    
    std::vector<Edge*>& getOutEdges(){
        return outEdges;
    }
    
    std::vector<Edge*>& getInEdges(){
        return inEdges;
    }
    
    Vertex(Read *read,bool BEGIN) {
        this->read=read;
        this->BEGIN=BEGIN;
    }
    
    /*  Sorted descending by length
     */
    static bool sortFunction(Edge* i,Edge *j){
        return i->length()<j->length();
    }
    
    void sortEdges(){
        std::sort(outEdges.begin(),outEdges.end(),sortFunction);
        std::sort(inEdges.begin(),inEdges.end(),sortFunction);
    }
    
};