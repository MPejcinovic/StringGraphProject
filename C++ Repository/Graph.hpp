

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

    std::string toGFA(){
        std::stringstream ss;
        
        ss<<"H\tVN:Z:1.0\n";
        for(Read *r:getReads()){
            ss << r->toGFA() << "\n";
        }
        for(Vertex *v:vertices){
            for(Edge *e:v->getOutEdges()){
                ss << e->toGFA() << "\n";
            }
        }
        return ss.str();
    }
    
    std::set<Read*> getReads(){
        std::set<Read*> arr;
        for(Vertex *v:vertices){
            for(Edge *e:v->getOutEdges()){
                for(Overlap *overlap:e->getOverlaps()){
                    arr.insert(overlap->f.getRead());
                    arr.insert(overlap->g.getRead());
                }
            }
        }
        return arr;
    }
    
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
    void createVertices(std::vector<Read*> reads){
        for(Read *r:reads){
            Vertex *B=new Vertex(r, true);
            Vertex *E=new Vertex(r, false);
            vertices.push_back(B);
            vertices.push_back(E);
            //new Edge(B, E, null, r, 0, r.length());
            //new Edge(E,B, null, r, 0, r.length());
        }
    }
    
    int i1=0,i2=0,i3=0,i4=0;
    
    void createEdge(Overlap *overlap,std::map<int, Vertex*> map){
        Read *f=overlap->f.getRead();
        Read *g=overlap->g.getRead();
        Vertex *FB=map[Vertex::hash(f, true)];
        Vertex *FE=map[Vertex::hash(f, false)];
        Vertex *GB=map[Vertex::hash(g, true)];
        Vertex *GE=map[Vertex::hash(g, false)];
        if(overlap->f.begin>=overlap->f.end){
            std::cout <<"EEEEEE";
            /*System.out.println(overlap.f.begin);
            System.out.println(overlap.f.length());
            System.out.println("EEEEE");*/
        }
        if(overlap->f.begin>0){
            if(overlap->g.begin<overlap->g.end){
                //System.out.println("I1:"+overlap);
                i1++;
                new Edge(GB, FB,overlap, f, overlap->f.begin, 0,1);
                new Edge(FE, GE,overlap, g, overlap->g.end, g->getLength(),1);
            }else{
                //System.out.println("I2:"+overlap);
                i2++;
                new Edge(GE, FB,overlap, f, overlap->f.begin, 0,21);
                new Edge(FE, GB, overlap,g, overlap->g.end, 0,22);
            }
        }
        else{
            if(overlap->g.begin<overlap->g.end){
                //System.out.println("I3:"+overlap);
                i3++;
                new Edge(FB, GB, overlap,g, overlap->g.begin, 0,3);
                new Edge(GE, FE, overlap,f, overlap->f.end, f->getLength(),3);
            }else{
                //System.out.println("I4:"+overlap);
                i4++;
                new Edge(FB, GE,overlap, g, overlap->g.begin,g->getLength(),4);
                new Edge(GB, FE, overlap,f, overlap->f.end, f->getLength(),4);
            }
        }
    }
    
    void createEdges(std::vector<Overlap*> overlaps){
        std::map<int, Vertex*> map;
        for(Vertex *v:vertices){
            if(map[v->hashCode()]!=NULL){
                std::cout <<"map entry != null\n";
            }
            map[v->hashCode()]=v;
        }
        for(Overlap *overlap:overlaps){
            createEdge(overlap, map);
        }
    }
    
public:
    void removeEmptyVertices(){
        auto it=vertices.begin();
        while(it!=vertices.end()){
            Vertex *v=*it;
            if(v->getInEdges().size()==0&&v->getOutEdges().size()==0){
                it=vertices.erase(it);
            }else{
                ++it;
            }
        }
    }
    
    Graph(std::vector<Read*> reads,std::vector<Overlap*> overlaps){
        createVertices(reads);
        createEdges(overlaps);
        std::cout << "I1:" << i1 << "\n";
        std::cout << "I2:" << i2 << "\n";
        std::cout << "I3:" << i3 << "\n";
        std::cout << "I4:" << i4 << "\n";
        std::cout <<toString()<<"\n";
        removeEmptyVertices();
        for(Vertex *v:vertices){
            v->sortEdges();
        }
    }
    
    void report() {
        for(Vertex *v:vertices){
            for(Edge *e:v->getOutEdges()){
                e->getOverlap()->f.isLeftPart();
                e->getOverlap()->g.isLeftPart();
            }
        }
    }

};