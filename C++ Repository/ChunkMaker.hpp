//
//  ChunkMaker.hpp
//  Bioinformatika
//
//  Created by Fran on 13/01/16.
//  Copyright © 2016 Fran. All rights reserved.
//

#pragma once
#include <set>
#include "Chunk.hpp"
#include "Graph.hpp"
#include "Read.hpp"
#include <vector>

class ChunkMaker {
    
private:
    
    
    int MIN_LENGTH=800;
    int MAX_LENGTH=14000;
    Chunk *currentChunk;
    Graph *graph;
    std::set<Read*> unusedReads;
    
public:
    
    std::vector<Vertex*> vertices;
    std::vector<Chunk*> chunks;
    
    
    
private:
    
    void vertexAdded(Vertex *v){
        
        int r=0;
        auto iterator=vertices.begin();
        while (iterator!=vertices.end()) {
            Vertex *i=*iterator;
            if(i->getRead()==v->getRead()){
                iterator=vertices.erase(iterator);
                r++;
            }else{
                ++iterator;
            }
        }
    }
    
    void addToCurrentChunkEnd(Vertex *v){
        currentChunk->vertices.push_back(v);
        vertexAdded(v);
    }
    
    void addToCurrentChunkBegin(Vertex *v){
        currentChunk->vertices.insert(currentChunk->vertices.begin(), v);
        vertexAdded(v);
    }
    
    bool incrementChunk(){
        std::vector<Edge*> outEdges=currentChunk->lastVertex()->getOutEdges();
        std::sort(outEdges.begin(),outEdges.end(),Vertex::sortFunction);
        
        if(true){
            for(Edge *e:outEdges){
                int len=e->segmentLength();
                if(len<MIN_LENGTH||len>MAX_LENGTH){
                    continue;
                }
                if(std::find(vertices.begin(), vertices.end(), e->getEndVertex())==vertices.end()){
                    continue;
                }
                addToCurrentChunkEnd(e->getEndVertex());
                currentChunk->edges.push_back(e);
                return true;
            }
        }
        
        std::vector<Edge*> inEdges=currentChunk->firstVertex()->getInEdges();
        std::sort(inEdges.begin(),inEdges.end(),Vertex::sortFunction);

        if(true){
            for(Edge *e:inEdges){
                int len=e->segmentLength();
                if(len<MIN_LENGTH||len>MAX_LENGTH){
                    continue;
                }
                if(std::find(vertices.begin(), vertices.end(), e->getStartVertex())==vertices.end()){
                    continue;
                }
                addToCurrentChunkBegin(e->getStartVertex());
                currentChunk->edges.insert(currentChunk->edges.begin(),e);
                return true;
            }
            
        }
        
        return false;
    }
    
public:
    
    void process(){
        unusedReads=graph->getReads();
        vertices=graph->getVertices();
        while(vertices.size()!=0){
            int maxIdx=0;
            size_t maxLen=-1;
            std::vector<Vertex*> vCopy=vertices;
            for(int i=0;i<vertices.size();i++){
                currentChunk=new Chunk();
                addToCurrentChunkEnd(vertices[i]);
                while(incrementChunk()){
                }
                size_t size=currentChunk->size();
                if(maxLen==-1||size>maxLen){
                    maxIdx=i;
                    maxLen=size;
                }
                delete currentChunk;
                vertices=vCopy;
            }
            vertices=vCopy;
            currentChunk=new Chunk();
            addToCurrentChunkEnd(vertices[maxIdx]);
            while(incrementChunk()){
            }
            chunks.push_back(currentChunk);
            
            //break;
        }
        std::sort(chunks.begin(), chunks.end(), Chunk::sortFunction);
    }
    
    ChunkMaker(Graph *graph){
        this->graph=graph;
    }

    ChunkMaker(){
    }

};