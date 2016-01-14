//
//  Chunk.hpp
//  Bioinformatika
//
//  Created by Fran on 13/01/16.
//  Copyright © 2016 Fran. All rights reserved.
//

#pragma once
#include <vector>
#include <string>
#include <sstream>
#include "Vertex.hpp"
#include "Edge.hpp"

class Chunk {
    
public:
    int idx;
    std::vector<Vertex*> vertices;
    std::vector<Edge*> edges;
    
    std::string toString() {
        int sum=0;
        std::stringstream ss;
        ss<<vertices[0]->getRead()->getID() << " :" << vertices[0]->getRead()->getLength() << "\n";
        sum+=vertices[0]->getRead()->getLength();
        for(int i=0;i<edges.size();i++){
            Edge *edge=edges[i];
            sum+=edge->length();
            ss << edge->toString() << " Sum:" << sum << "\n";
        }
        return ss.str();
    }
    
    Vertex* lastVertex(){
        return vertices[vertices.size()-1];
    }
    
    Vertex* firstVertex(){
        return vertices[0];
    }
    
    std::string toGFA(){
        std::stringstream ss;
        if(!vertices[0]->BEGIN){
            ss << vertices[0]->getRead()->sequence;
        }else{
            std::string str=Read::complement(vertices[0]->getRead()->sequence);
            std::reverse(str.begin(),str.end());
            ss << str;
        }
        for(int i=0;i<edges.size();i++){
            Edge *edge=edges[i];
            if(edge->getStartVertex()!=vertices[i]){
                exit(-2);
            }
            ss << edge->toDNA();
        }
        return ss.str();
    }
    
    static bool sortFunction(Chunk& i,Chunk& j){
        return i.size()>j.size();
    }
    
    Chunk(){
        
    }
    
    size_t size() {
        return toGFA().length();
    }
    
};