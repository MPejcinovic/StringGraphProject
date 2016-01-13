

#pragma once
#include <iostream>
#include "Read.hpp"
#include <string>
#include <sstream>

class OverlapPart {
    
public:
    Read *read;
    int begin;
    int end;
    int originalBegin;
    int originalEnd;
    
    static int absoluteMargin;
    static double percentMargin;
    
    
    std::string toString() {
        std::stringstream ss;
        ss << "{read:" << read->toString() << "begin:" << begin << ",end:" << end << "}";
        return ss.str();
    }
    
    bool isForward(){
        return begin<=end;
    }
    
    bool isLeftPart(){
        return (isForward()&&begin!=0)||(!isForward()&&begin!=read->getLength()-1);
    }
    
    bool isRightPart(){
        return !isLeftPart();
    }
    
    OverlapPart(Read *read,int begin,int end, int originalBegin, int originalEnd){
        this->read=read;
        this->begin=begin;
        this->end=end;
        this->originalBegin=originalBegin;
        this->originalEnd=originalEnd;
    }
    
    Read* getRead(){
        return read;
    }
    
    int length(){
        int length=end-begin;
        if(length<0){
            length=-length;
        }
        length++;
        return length;
    }
    
    bool isContainment(){
        return //length()==read.length();
        length()>=read->getLength()*(1-percentMargin/100)-absoluteMargin;
    }
    
    bool containsRead(Read *read) {
        return this->read==read;
    }

};