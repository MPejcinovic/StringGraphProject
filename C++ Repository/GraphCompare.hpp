

#pragma once
#include "Graph.hpp"
#include <iostream>
#include <unordered_set>
#include <vector>
#include <fstream>

class GraphCompare {
    
    
public:
    static void compare(Graph &graph,std::string fileName,std::string solutionFileName){
        std::unordered_set<int> indices;
        for(auto& iterMap : graph.idToVertexMap){
            Vertex *v=iterMap.second;
            for(Edge *edge:v->edges){
                indices.insert(edge->overlapIDX);
            }
        }
        std::vector<std::string> myLines;
        std::vector<std::string> solutionLines;
        std::ifstream infile(fileName);
        std::string line;
        int idx=0;
        while (std::getline(infile, line))
        {
            if(indices.find(idx)!=indices.end()){
                myLines.push_back(line);
            }
            idx++;
        }
        std::ifstream solFIle(solutionFileName);

        while (std::getline(solFIle, line))
        {
            solutionLines.push_back(line);
        }
        int count=0;
        for(std::string str:myLines){
            if(std::find(solutionLines.begin(), solutionLines.end(), str)!=solutionLines.end()){
                count++;
            }
        }
        std::cout << "CCCC:"<<1.0*count/solutionLines.size() << "\n";
        
    }
};