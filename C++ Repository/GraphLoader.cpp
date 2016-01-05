

#include "GraphLoader.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

void GraphLoader::getOverlaps(std::string fileName,int& numReads,std::vector<Overlap>& overlaps, std::unordered_set<int>& containedIDs){
    std::ifstream in_stream;
    in_stream.open(fileName);
    numReads=0;
    double garbage;
    int containedID;
    int complemented;
    int idx=0;
    while(!in_stream.eof()){
        Overlap overlap;
        in_stream >> overlap.A.id;
        in_stream >> overlap.B.id;
        
        in_stream >> garbage;
        in_stream >> garbage;
        
        in_stream >> complemented;
        if(!complemented){
            in_stream >> overlap.A.begin;
            in_stream >> overlap.A.end;
        }else{
            in_stream >> overlap.A.end;
            in_stream >> overlap.A.begin;
        }
        in_stream >> overlap.A.length;
        
        in_stream >> complemented;
        if(!complemented){
            in_stream >> overlap.B.begin;
            in_stream >> overlap.B.end;
        }else{
            in_stream >> overlap.B.end;
            in_stream >> overlap.B.begin;
        }
        in_stream >> overlap.B.length;
        if(complemented){
            int b=overlap.B.length-overlap.B.end;
            int e=overlap.B.length-overlap.B.begin;
            overlap.B.begin=b;
            overlap.B.end=e;
        }
        if(overlap.A.id>numReads){
            numReads=overlap.A.id;
        }
        if(overlap.B.id>numReads){
            numReads=overlap.B.id;
        }
        if(overlap.isContainment(containedID)){
            //std::cout <<"CONTAINMENT!";
            containedIDs.insert(containedID);
        }
        overlap.idx=idx;
        overlaps.push_back(overlap);
        idx++;
    }
    in_stream.close();
}