

#pragma once
#include "Overlap.hpp"
#include <unordered_set>
#include <vector>
#include <set>

class GraphLoader {
    
    
public:
    static void getOverlaps(std::string fileName,int& numReads,std::vector<Overlap>& overlaps, std::unordered_set<int>& containedIDs);
};