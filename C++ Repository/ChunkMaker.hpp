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
    Chunk currentChunk;
    Graph *graph;
    
public:
    
    std::vector<Vertex*> vertices;
    std::vector<Chunk> chunks;
    
    
    
private:
    
    void vertexAdded(Vertex *v){
        auto iterator=vertices.begin();
        while (iterator!=vertices.end()) {
            Vertex *i=*iterator;
            if(i->getRead()==v->getRead()){
                iterator=vertices.erase(iterator);
            }else{
                ++iterator;
            }
        }
    }
    
    void addToCurrentChunkEnd(Vertex *v){
        currentChunk.vertices.push_back(v);
        vertexAdded(v);
    }
    
    void addToCurrentChunkBegin(Vertex *v){
        currentChunk.vertices.insert(currentChunk.vertices.begin(), v);
        vertexAdded(v);
    }
    
    /*  Tries to increment the chunk by adding an edge to it, either to it's beginning or it's end
     */
    bool incrementChunk(){
        std::vector<Edge*>& outEdges=currentChunk.lastVertex()->getOutEdges();
        
        if(true){
            for(Edge *e:outEdges){
                int len=e->segmentLength();
                if(len>MAX_LENGTH){
                    continue;
                }
                if(std::find(vertices.begin(), vertices.end(), e->getEndVertex())==vertices.end()){
                    continue;
                }
                addToCurrentChunkEnd(e->getEndVertex());
                currentChunk.edges.push_back(e);
                return true;
            }
        }
        
        std::vector<Edge*>& inEdges=currentChunk.firstVertex()->getInEdges();

        if(true){
            for(Edge *e:inEdges){
                int len=e->segmentLength();
                if(len>MAX_LENGTH){
                    continue;
                }
                if(std::find(vertices.begin(), vertices.end(), e->getStartVertex())==vertices.end()){
                    continue;
                }
                addToCurrentChunkBegin(e->getStartVertex());
                currentChunk.edges.insert(currentChunk.edges.begin(),e);
                return true;
            }
            
        }
        
        return false;
    }
    
public:
    
    
    void process(){
        vertices=graph->getVertices();
        for(Vertex *v :vertices){ //we found that the algorithm functions best if the in and out edges are sorted ascending by length
            std::sort(v->getInEdges().begin(),v->getInEdges().end(),Vertex::sortFunction);
            std::sort(v->getOutEdges().begin(),v->getOutEdges().end(),Vertex::sortFunction);
        }
        
        while(vertices.size()!=0){
            int maxIdx=0;
            size_t maxLen=-1;
            std::vector<Vertex*> vCopy=vertices; //make a copy of vertices
            
            for(int i=0;i<vertices.size();i++){ //find the biggest chunk we can make at this point
                currentChunk=Chunk();
                addToCurrentChunkEnd(vertices[i]); //make a chunk starting with vertex at index i
                while(incrementChunk()){
                }
                size_t size=currentChunk.size();
                if(maxLen==-1||size>maxLen){//remember the largest made chunk
                    maxIdx=i;
                    maxLen=size;
                }
                vertices=vCopy; //restore the vertices
            }
            
            currentChunk=Chunk();
            addToCurrentChunkEnd(vertices[maxIdx]);
            while(incrementChunk()){
            }
            chunks.push_back(currentChunk);
        }
        std::sort(chunks.begin(), chunks.end(), Chunk::sortFunction);
    }
    
    ChunkMaker(Graph *graph){
        this->graph=graph;
    }

    ChunkMaker(){
    }

};