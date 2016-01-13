

#pragma once
#include "OverlapPart.hpp"
#include <string>
#include <sstream>

class Overlap {
public:
    int IDX;
    OverlapPart f;
    OverlapPart g;
    
    std::string toString() {
        std::stringstream ss;
        ss << "{f:" << f.toString() << ",g:" << g.toString() << "}";
        return ss.str();
    }
    
    OverlapPart leftPart(){
        if(f.isLeftPart()){
            return f;
        }
        return g;
    }
    
    OverlapPart rightPart(){
        if(f.isRightPart()){
            return f;
        }
        return g;
    }
    
    Overlap(int IDX,OverlapPart f,OverlapPart g):g(g),f(f){
        this->IDX=IDX;
    }
    
    bool containsRead(Read *read){
        return f.containsRead(read)||g.containsRead(read);
    }
    
    Read* getContainedRead(){
        if(g.isContainment()&&f.isContainment()){
            return NULL;
        }
        if(f.isContainment()){
            return f.getRead();
        }
        if(g.isContainment()){
            return g.getRead();
        }
        return NULL;
    }
    
    int size() {
        return f.length();
    }
};