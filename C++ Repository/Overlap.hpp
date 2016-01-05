

#pragma once
#include "OverlapPart.hpp"

class Overlap {
public:
    int idx;
    OverlapPart A;
    OverlapPart B;
    bool isContainment(int &containedID);
};