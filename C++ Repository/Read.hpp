//
//  Read.hpp
//  Bioinformatika
//
//  Created by Fran on 12/01/16.
//  Copyright © 2016 Fran. All rights reserved.
//

#pragma once
#include <string>
#include <algorithm>
#include <sstream>
#include <iostream>

class Read {
public:
    int REFERENCE=2;
    int ID;
    int length;
    std::string sequence;
    
public:
    
    ~Read(){
    }
    static std::string complement(std::string str){
        std::string string="";
        for(int i=0;i<str.length();i++){
            if(str[i]=='A'){
                string+='T';
                continue;
            }
            if(str[i]=='T'){
                string+='A';
                continue;
            }
            if(str[i]=='G'){
                string+='C';
                continue;
            }
            if(str[i]=='C'){
                string+='G';
                continue;
            }
        }
        return string;
    }
    
    std::string get(int begin,int end){
        if(begin<end){
            return sequence.substr(begin, end-begin);
        }
        if(begin==end){
            return "";
        }
        std::string str=complement(sequence.substr(end+1, begin-end-1));
        std::reverse(str.begin(), str.end());
        return str;
    }
    
    std::string toGFA(){
        std::stringstream ss;
        ss << "S\t"<< getID() << "\t" << sequence;
        return ss.str();
    }
    
    std::string toString() {
        std::stringstream ss;
        ss << "{ID:" << ID <<",length:" << length <<"}";
        return ss.str();
    }
    
    
    int hashCode() {
        return ID;
    }
    
    int getLength(){
        return length;
    }
    
    int getID(){
        return ID;
    }
    
    Read(int ID,int length){
        this->ID=ID;
        this->length=length;
    }
    
};