//
//  main.cpp
//  Bioinformatika
//
//  Created by Fran on 17/12/15.
//  Copyright © 2015 Fran. All rights reserved.
//

#include <string>
#include <vector>
#include <unordered_set>
#include "Overlap.hpp"
#include "Graph.hpp"
#include "GraphLoader.hpp"
#include <iostream>
#include "TransitiveEdgeRemover.hpp"
#include "InternalVertexRemover.hpp"
#include "NetworkFlowAnalyzer.hpp"
#include "GraphCompare.hpp"
#include "ChunkMaker.hpp"
#include <fstream>
#include <math.h>

std::string filePath="/Users/Stanic/Google Drive/Projekti/Bioinformatika/overlaps.mhap";
std::string readsFileName="/Users/Stanic/Google Drive/Projekti/Bioinformatika/reads.fq";
std::string output="/Users/Stanic/Google Drive/Projekti/Bioinformatika/chunks45.fq";
std::string bandageGraphOutput="/Users/Stanic/Google Drive/Projekti/Bioinformatika/bG.fq";
std::string bandageChunksOutput="/Users/Stanic/Google Drive/Projekti/Bioinformatika/bC.fq";

ChunkMaker chunkMaker;
Graph *graph;

void write45All(){
    std::ofstream file(output);
    for(int i=0;i<chunkMaker.chunks.size();i++){
        Chunk& cr=chunkMaker.chunks[i];
        cr.idx=i;
        if(cr.edges.size()!=0){
            std::string GFA=cr.toGFA();
            file << "S\t" << (i) << "\t" << GFA << "\n";
        }
    }
    file.close();
}

void writeBandageChunks(){
    std::ofstream file(bandageChunksOutput);
    file <<"H\tVN:Z:1.0\n";
    std::set<Read*> reads;
    for(Chunk c:chunkMaker.chunks){
        for(Vertex *v:c.vertices){
            if(reads.find(v->getRead())==reads.end()){
                file << v->getRead()->toGFA() << "\n";
                reads.insert(v->getRead());
            }
        }
        for(Edge *e:c.edges){
            file << e->toGFARecursive() << "\n";
        }
    }
    file.close();
}

void writeBandageGraph(){
    std::ofstream file(bandageGraphOutput);
    file << graph->toGFA();
    file.close();
}

int main(int argc, const char * argv[]){
    if(argc!=6){
       std::cout << "Arguments: <overlapsFile> <readsFile> <gepardGFAOutput> <bandageGraphOutput> <bandageChunksOutput>" << "\n";
       exit(-1);
    }
    filePath=argv[1];
    readsFileName=argv[2];
    output=argv[3];
    bandageGraphOutput=argv[4];
    bandageChunksOutput=argv[5];
    GraphLoader loader;
    
    graph=loader.getGraph(filePath,readsFileName);
    std::cout << "After containment:" << graph->toString() << "\n";
    
    TransitiveEdgeRemover transitiveEdgeRemover(graph);
    transitiveEdgeRemover.process();
    std::cout << "After transitive:" << graph->toString()<< "\n";
    graph->removeEmptyVertices();
    std::cout << "After removing empty:" << graph->toString()<< "\n";
    
    InternalVertexRemover internalVertexRemover(graph);
    internalVertexRemover.process();
    std::cout << "After internal:" << graph->toString()<< "\n";
    graph->removeEmptyVertices();
    std::cout << "After removing empty:" << graph->toString()<< "\n";
    
    NetworkFlowAnalyzer networkFlowAnalyzer(graph);
    networkFlowAnalyzer.process();
    
    std::cout << "After network:" << graph->toString()<< "\n";
    graph->removeEmptyVertices();
    std::cout << "After removing empty:" << graph->toString()<< "\n";
				
    
    chunkMaker=ChunkMaker(graph);
    chunkMaker.process();
    write45All();
    writeBandageChunks();
    writeBandageGraph();
    int size=0;
    int a=0;
    for(Chunk& c:chunkMaker.chunks){
        size+=c.size();
        if(c.edges.size()==0){
            a++;
        }
    }
    std::cout << "One vertex chunks:" << a << "\n";
    std::cout << "All chunks combined size:" << size << "\n";
    int N50=0;
    size_t lastSize=0;
    for(Chunk& c:chunkMaker.chunks){
        N50+=c.size();
        lastSize=c.size();
        if(N50>0.5*size){
            break;
        }
    }
    delete graph;
    std::cout << "N50:" << lastSize << "\n";
    std::cout << "Number of chunks" << chunkMaker.chunks.size() << "\n";
    return 0;
}
