

#pragma once
#include <iostream>

class OverlapPart {
    
public:
    int id;
    int begin;
    int end;
    int length;
    
    static int containmentMargin;
    static double containmentRelativeMargin;
    
    bool isContainment(){
        int overlapLength=end-begin;
        if(overlapLength<0){
            overlapLength=-overlapLength;
        }
        if(begin<overlapLength*0.02){
            begin=0;
        }
        if(end<overlapLength*0.02){
            end=0;
        }
        //std::cout << "LEN:"<<length<<" OVERL:"<<overlapLength<<"\n";
        return overlapLength>=length-containmentMargin-length*containmentRelativeMargin;
    }
};