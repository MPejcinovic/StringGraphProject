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
            //System.out.println("A");
            Edge *edge=edges[i];
            if(edge->getStartVertex()!=vertices[i]){
                exit(-1);
            }
            sum+=edge->length();
            ss << edge->toString() << " Sum:" << sum << "\n";
            /*
             Read read=edge.getEndVertex().getRead();
             Overlap overlap=edge.getOverlap();
             OverlapPart part=overlap.f;
             if(part.getRead()!=read){
             part=overlap.g;
             }
             
             if(part.isForward()){
             b.append(read.get(part.end, read.length()-1));
             }else{
             b.append(read.get(part.begin,0));
             }*/
        }
        return ss.str();
    }
    
    Vertex* lastVertex(){
        return vertices[vertices.size()-1];
    }
    
    Vertex* firstVertex(){
        return vertices[0];
    }
    
    std::string toGFA60(){
        std::stringstream ss;
        for(Vertex *v:vertices){
            ss << v->getRead()->sequence;
        }
        return ss.str();
    }
    
    std::string toGFA(){
        std::stringstream ss;
        //b.append(vertices.get(0).getRead().sequence);
        if(!vertices[0]->BEGIN){
            ss << vertices[0]->getRead()->sequence;
        }else{
            std::string str=Read::complement(vertices[0]->getRead()->sequence);
            std::reverse(str.begin(),str.end());
            ss << str;
        }
        //b.append(vertices.get(0).getRead().sequence);
        for(int i=0;i<edges.size();i++){
            //System.out.println("A");
            Edge *edge=edges[i];
            if(edge->getStartVertex()!=vertices[i]){
                exit(-2);
            }
            ss << edge->toDNA();
            /*
             Read read=edge.getEndVertex().getRead();
             Overlap overlap=edge.getOverlap();
             OverlapPart part=overlap.f;
             if(part.getRead()!=read){
             part=overlap.g;
             }
             
             if(part.isForward()){
             b.append(read.get(part.end, read.length()-1));
             }else{
             b.append(read.get(part.begin,0));
             }*/
        }
        return ss.str();
    }
    
    static bool sortFunction(Chunk* i,Chunk *j){
        return i->size()>j->size();
    }
    
    Chunk(){
        
    }
    
    size_t size() {
        return toGFA().length();
    }
    
};