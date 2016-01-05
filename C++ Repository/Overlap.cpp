
#include "Overlap.hpp"


bool Overlap::isContainment(int &containedID){
    if(A.isContainment()){
        containedID=A.id;
        return true;
    }
    if(B.isContainment()){
        containedID=B.id;
        return true;
    }
    return false;
}
