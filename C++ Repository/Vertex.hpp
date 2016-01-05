

#pragma once
#include <vector>
#include "Edge.hpp"
#include <iostream>
#include <sstream>

class Vertex {
    
public:
    int id;
    char mark;
    bool isBegin;
    std::vector<Edge*> edges;
    std::vector<Edge*> inEdges;
    
    int getHash(){
        if(isBegin){
            return id;
        }
        return -id;
    }
    
    std::string toString(){
        std::stringstream ss;
        ss <<"Vertex:"<<id << " Reversed:" << isBegin <<"\n";
        ss <<"In edges:\n";
        for(Edge* edge :inEdges){
            ss<<edge->toString();
        }
        ss <<"Out edges:";
        for(Edge* edge :edges){
            ss<<edge->toString();
        }
        return ss.str();
    }
    
    ~Vertex(){
        for(Edge *edge:edges){
            delete edge;
        }
    }
    
    void addEdge(Edge* edge){
        edges.push_back(edge);
    }
    
    static bool sortFunction(Edge* i,Edge *j){
        return i->length<j->length;
    }
    
    void sortEdges(){
        //std::cout << "SB\n";
        std::sort(edges.begin(),edges.end(),sortFunction);
        /*for(Edge* edge :edges){
            std::cout <<edge->length <<"\n";
        }
        std::cout << "SE\n";*/

    }
    
};