

#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "Overlap.hpp"
#include "Vertex.hpp"
#include "Edge.hpp"
#include <iostream>
#include "GraphLoader.hpp"
#include <set>
#include <chrono>
#include <sstream>

class Graph {
private:
    int originalNumReads;
    
    Vertex* getVertex(int ID,bool isBegin){
        if(isBegin){
            return idToVertexMap[ID];
        }
        return idToVertexMap[-ID];
    }
    
    void createVertices(int numReads,std::unordered_set<int>& containedIDs){
        std::cout << "Num reads:"<<numReads<<"\n";
        for(int i=1;i<=numReads;i++){
            if(containedIDs.find(i)!=containedIDs.end()){//i is removed
                continue;
            }
            Vertex *begin=new Vertex;
            begin->id=i;
            begin->isBegin=true;
            idToVertexMap[begin->getHash()]=begin;
            
            Vertex* end=new Vertex;
            end->id=i;
            end->isBegin=false;
            idToVertexMap[end->getHash()]=end;
        }
    }
    
    void createEdges(Overlap& overlap){
        Vertex *F_B=getVertex(overlap.A.id, true);
        Vertex *F_E=getVertex(overlap.A.id, false);
        Vertex *G_B=getVertex(overlap.B.id, true);
        Vertex *G_E=getVertex(overlap.B.id, false);
        if(overlap.A.begin>0){
            if(overlap.B.begin<overlap.B.end){
                new Edge(G_B,F_B, overlap.A.id, overlap.A.begin, 0,overlap.idx);
                new Edge(F_E,G_E,overlap.B.id,overlap.B.end,overlap.B.length,overlap.idx);
            }else{
                //std::cout <<"Start:"<<G_E<<"\n";
                //std::cout <<"Start:"<<F_B<<"\n";
                new Edge(G_E,F_B,overlap.A.id,overlap.A.begin,0,overlap.idx);
                new Edge(F_E,G_B,overlap.B.id,overlap.B.end,0,overlap.idx);
            }
        }else{
            if(overlap.B.begin<overlap.B.end){
                new Edge(F_B,G_B,overlap.B.id,overlap.B.begin,0,overlap.idx);
                new Edge(G_E,F_E,overlap.A.id,overlap.A.end,overlap.A.length,overlap.idx);
            }else{
                new Edge(F_B,G_E,overlap.B.id,overlap.B.length,overlap.B.begin,overlap.idx);
                new Edge(G_B,F_E,overlap.A.id,overlap.A.end,overlap.A.length,overlap.idx);
            }
        }
    }
    
    void sortEdges(){
        for(auto& entry: idToVertexMap){
            entry.second->sortEdges();
        }
    }

    void removeEmptyVertices(){
        auto iterator=idToVertexMap.begin();
        while(iterator!=idToVertexMap.end()){
            Vertex *V=(*iterator).second;
            if(V->edges.size()==0&&V->inEdges.size()==0){
                iterator=idToVertexMap.erase(iterator);
                delete V;
            }else{
                ++iterator;
            }
        }
    }
    
    void initialize(int numReads,std::vector<Overlap>& overlaps, std::unordered_set<int>& containedIDs){
        this->originalNumReads=numReads;
        std::cout << "Creating vertices\n";
        createVertices(numReads,containedIDs);
        std::cout << "Creating edges\n";
        for(Overlap& overlap :overlaps){
            if(containedIDs.find(overlap.A.id)!=containedIDs.end()//A is contained
               ||containedIDs.find(overlap.B.id)!=containedIDs.end()){//B is contained
                continue;
            }
            createEdges(overlap);
        }
        removeEmptyVertices();
        sortEdges();
    }
    
public:
    std::unordered_map<int, Vertex*> idToVertexMap;

    std::string toString(){
        std::stringstream ss;
        for(auto& entry:idToVertexMap){
            Vertex *V=entry.second;
            ss << V->toString();
        }
        return ss.str();
    }
    
    std::string shortDescription(){
        std::stringstream ss;
        ss << "Num vertices:"<< idToVertexMap.size() << " Num edges:" << getNumberOfEdges();
        return ss.str();
    }
    
    int getNumberOfEdges(){
        int sum=0;
        for(auto& entry:idToVertexMap){
            Vertex *V=entry.second;
            sum+=V->edges.size();
        }
        return sum;
    }
    
    size_t numVertices(){
        return idToVertexMap.size();
    }
    
    size_t getNumReads(){
        return idToVertexMap.size()/2;
    }

    int getOriginalNumReads(){
        return originalNumReads;
    }

    Graph(int numReads,std::vector<Overlap>& overlaps, std::unordered_set<int>& containedIDs){
        initialize(numReads, overlaps, containedIDs);
    }
    
    Graph(std::string fileName){
        int numReads=0;
        std::vector<Overlap> overlaps;
        std::unordered_set<int> containedIDs;
        std::cout << "Loading\n";
        std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds >(
                                                        std::chrono::system_clock::now().time_since_epoch()
                                                        );
        GraphLoader::getOverlaps(fileName, numReads, overlaps, containedIDs);
        std::chrono::milliseconds ms2 = std::chrono::duration_cast<std::chrono::milliseconds >(
                                                                                              std::chrono::system_clock::now().time_since_epoch()
                                                                                              );
        std::cout << "Loaded, numReads="<<numReads<<"\n";
        std::cout << "numOverlaps="<<overlaps.size()<<"\n";
        std::cout <<"Time:" << (ms2.count()-ms.count())<<"\n";
        initialize(numReads,overlaps,containedIDs);
    }
    
    
    ~Graph(){
        for(auto& iterMap : idToVertexMap){
            delete iterMap.second;
        }
    }
};