

#pragma once
#include <vector>
#include <map>
#include <unordered_set>
#include "Overlap.hpp"
#include "Vertex.hpp"
#include "Edge.hpp"
#include <iostream>
#include <set>
#include <chrono>
#include "Read.hpp"
#include <sstream>

class Graph {
    
public:
    std::vector<Vertex*> vertices;

    ~Graph(){
        for(Vertex *v :vertices){
            delete v;
        }
    }
    
    std::string toGFA(){
        std::stringstream ss;
        
        ss<<"H\tVN:Z:1.0\n";
        for(Read *r:getReads()){
            ss << r->toGFA() << "\n";
        }
        for(Vertex *v:vertices){
            for(Edge *e:v->getOutEdges()){
                ss << e->toGFARecursive() << "\n";
            }
        }
        return ss.str();
    }
    
    /*  Calculates all reads present in the graph (referenced by it's edges)
     */
    std::set<Read*> getReads(){
        std::set<Read*> arr;
        for(Vertex *v:vertices){
            for(Edge *e:v->getOutEdges()){
                for(Overlap *overlap:e->getOverlaps()){
                    arr.insert(overlap->f->getRead());
                    arr.insert(overlap->g->getRead());
                }
            }
        }
        return arr;
    }
    
    /*  Calculates all overlaps present in the graph (referenced by it's edges)
     */
    std::set<Overlap*> getOverlaps(){
        std::set<Overlap*> arr;
        for(Vertex *v:vertices){
            for(Edge *e:v->getOutEdges()){
                std::vector<Overlap*> overlaps=e->getOverlaps();
                arr.insert(overlaps.begin(),overlaps.end());
            }
        }
        return arr;
    }
    
    size_t numVertices(){
        return vertices.size();
    }
    
    int numEdges(){
        int s=0;
        for(Vertex *v:vertices){
            s+=v->getOutEdges().size();
        }
        return s;
    }
    

    std::string toString() {
        std::stringstream ss;
        ss<<"|V|=" << numVertices() << ",|E|=" << numEdges();
        return ss.str();
    }
    
    std::vector<Vertex*> getVertices(){
        return vertices;
    }
    
private:
    
    /*  Creates the initial vertices of the graph using the provided reads,
     *  for each read 2 vertices are created
     */
    void createVertices(std::map<int,Read*>& reads){
        vertices=std::vector<Vertex*>(reads.size()*2);
        int idx=0;
        auto iterator=reads.begin();
        while(iterator!=reads.end()){
            Read *r=iterator->second;
            Vertex *B=new Vertex(r, true);
            Vertex *E=new Vertex(r, false);
            vertices[idx*2]=B;
            vertices[idx*2+1]=E;
            idx++;
            ++iterator;
        }
    }
    
    
    /*  Creates 2 edge of the graph using the provided overlap and vertices
     */
    void createEdge(Overlap *overlap,std::map<int, Vertex*> map){
        Read *f=overlap->f->getRead();
        Read *g=overlap->g->getRead();
        Vertex *FB=map[Vertex::hash(f, true)];
        Vertex *FE=map[Vertex::hash(f, false)];
        Vertex *GB=map[Vertex::hash(g, true)];
        Vertex *GE=map[Vertex::hash(g, false)];
        if(overlap->f->begin>0){
            if(overlap->g->begin<overlap->g->end){
                new Edge(GB, FB,overlap, f, overlap->f->begin, 0,1);
                new Edge(FE, GE,overlap, g, overlap->g->end, g->getLength(),1);
            }else{
                new Edge(GE, FB,overlap, f, overlap->f->begin, 0,21);
                new Edge(FE, GB, overlap,g, overlap->g->end, 0,22);
            }
        }
        else{
            if(overlap->g->begin<overlap->g->end){
                new Edge(FB, GB, overlap,g, overlap->g->begin, 0,3);
                new Edge(GE, FE, overlap,f, overlap->f->end, f->getLength(),3);
            }else{
                new Edge(FB, GE,overlap, g, overlap->g->begin,g->getLength(),4);
                new Edge(GB, FE, overlap,f, overlap->f->end, f->getLength(),4);
            }
        }
    }
    
    
    /*  Creates the edge of the graph using the provided overlaps,
     *  for each overlap 2 edges are created as described in the string graph paper
     */
    void createEdges(std::vector<Overlap*>& overlaps){
        std::map<int, Vertex*> map;
        for(Vertex *v:vertices){
            map[v->hashCode()]=v;
        }
        for(Overlap *overlap:overlaps){
            if(overlap!=NULL){
                createEdge(overlap, map);   
            }
        }
    }
    
public:
    
    /*  Removes all vertices with no in-edges and no out-edges from the graph
     */
    void removeEmptyVertices(){
        auto it=vertices.begin();
        while(it!=vertices.end()){
            Vertex *v=*it;
            if(v->getInEdges().size()==0&&v->getOutEdges().size()==0){
                it=vertices.erase(it);
                delete v;
            }else{
                ++it;
            }
        }
    }
    
    Graph(std::map<int,Read*> reads,std::vector<Overlap*> overlaps){
        createVertices(reads);
        createEdges(overlaps);
        std::cout <<toString()<<"\n";
        removeEmptyVertices();
        for(Vertex *v:vertices){
            v->sortEdges();
        }
    }
    
};