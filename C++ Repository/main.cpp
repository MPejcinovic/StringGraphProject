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
std::string solutionPath="/Users/Stanic/Google Drive/Projekti/Bioinformatika/overlaps.after_unitigger.mhap";
std::string savePath="output";
std::string readsFileName="/Users/Stanic/Google Drive/Projekti/Bioinformatika/reads.fq";
ChunkMaker chunkMaker;


void write60(){
    std::ofstream file("/Users/Stanic/Google Drive/Projekti/Bioinformatika/chunks60.gfa");
    int arr[]={3,-2,10,-35,31,19,34,-20,14,-60,-49,6,8,-4,17,-7,-0,11,25,-18,-23,-16,-42,30,-40,39,32};
    for(int i=0;i<sizeof(arr)/sizeof(arr[0]);i++){
        int idx=arr[i];
        int offset=0;
        Chunk *cr=chunkMaker.chunks[abs(idx)+offset];
        cr->idx=idx+offset;
        std::string GFA=cr->toGFA60();
        if(idx<0||idx==0){
            std::reverse(GFA.begin(), GFA.end());
            GFA=Read::complement(GFA);
        }
        file << "S\t" << (idx+offset) << "\t" << GFA << "\n";
    }
    file.close();
}

void write45(){
    std::ofstream file("/Users/Stanic/Google Drive/Projekti/Bioinformatika/chunks45.gfa");
    //int[] arr=new int[]{3,-2,10,-35,31,19,34,-20,14,-60,-49,6,8,-4,17,-7,-0,11,25,-18,-23,-16,-42,30,-40,39,32};
    //int[] arr=new int[]{3,-2,9,4,36,-34,22,10,-20,39,-25,17,11,6,-5,7,30,-12,-1,15,-23,-32,-21,24,-8,-19,-25,44,38,42};
    
    int arr[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14};
    for(int i=0;i<sizeof(arr)/sizeof(arr[0]);i++){
        int idx=arr[i];
        int offset=0;
        Chunk *cr=chunkMaker.chunks[abs(idx)+offset];
        cr->idx=idx+offset;
        std::string GFA=cr->toGFA();
        if(idx<0||idx==0){
            std::reverse(GFA.begin(), GFA.end());
            GFA=Read::complement(GFA);
        }
        file << "S\t" << (idx+offset) << "\t" << GFA << "\n";
    }
    file.close();
}

void write45All(){
    std::ofstream file("/Users/Stanic/Google Drive/Projekti/Bioinformatika/chunks45.gfa");
    for(int i=0;i<chunkMaker.chunks.size();i++){
        Chunk *cr=chunkMaker.chunks[i];
        cr->idx=i;
        if(cr->edges.size()!=0){
            std::string GFA=cr->toGFA();
            file << "S\t" << (i) << "\t" << GFA << "\n";
        }
    }
    file.close();
}

void test(){
    std::ofstream file("/Users/Stanic/Google Drive/Projekti/Bioinformatika/chunks45.gfa");
    int chunks[]={0,12,-1,-8,-5,1,2,-0,-4,1,-7,9,-3,-6,11,-2,-1,7,-10,-5,14,0};
    int begins[]={543250,0,0,0,0,300000,300000,1260340,0,75000,160000,0,0,0,0,0,660000,0,0,110000,0,0};
    int ends[]={1262070,0,100000,0,100000,700000,0,0,0,300000,0,0,0,0,0,315000,0,150000,0,0,0,543250};
    
    for(int i=0;i<sizeof(chunks)/sizeof(chunks[0]);i++){
        int idx=chunks[i];
        Chunk *cr=chunkMaker.chunks[abs(idx)];
        cr->idx=i;
        int begin=begins[i];
        size_t end=ends[i];
        if(end==0){
            end=cr->size();
        }
        std::string GFA=cr->toGFA();
        if(i==0||idx>0||i==sizeof(chunks)/sizeof(chunks[0])-1){
            GFA=GFA.substr(begin, end-begin);
        }else{
            GFA=Read::complement(GFA.substr(begin,end-begin));
            std::reverse(GFA.begin(), GFA.end());
        }
        file << "S\t" << (i) << "\t" << GFA << "\n";
    }
    file.close();
}

int main(int argc, const char * argv[]){
    GraphLoader loader;
    
    Graph *graph=loader.getGraph(filePath,readsFileName);
   // System.out.println("Memory:"+(Runtime.getRuntime().totalMemory()/(1024*1024)));
    std::cout << "After containment:" << graph->toString() << "\n";
    
    TransitiveEdgeRemover transitiveEdgeRemover(graph);
    transitiveEdgeRemover.process();
    std::cout << "After transitive:" << graph->toString()<< "\n";
    graph->removeEmptyVertices();
    std::cout << "After removing empty:" << graph->toString()<< "\n";
    
    //System.out.println("Memory:"+(Runtime.getRuntime().totalMemory()/(1024*1024)));
    
    InternalVertexRemover internalVertexRemover(graph);
    internalVertexRemover.process();
    std::cout << "After internal:" << graph->toString()<< "\n";
    graph->removeEmptyVertices();
    std::cout << "After removing empty:" << graph->toString()<< "\n";
    
    //System.out.println("Memory:"+(Runtime.getRuntime().totalMemory()/(1024*1024)));
    
    NetworkFlowAnalyzer networkFlowAnalyzer(graph);
    networkFlowAnalyzer.process();
    
    std::cout << "After network:" << graph->toString()<< "\n";
    graph->removeEmptyVertices();
    std::cout << "After removing empty:" << graph->toString()<< "\n";
				
    
    chunkMaker=ChunkMaker(graph);
    chunkMaker.process();
    write45All();
    int size=0;
    int a=0;
    for(Chunk *c:chunkMaker.chunks){
        //System.out.println(c.size());
        size+=c->size();
        if(c->edges.size()==0){
            a++;
        }
    }
    std::cout << "One vertex chunks:" << a << "\n";
    std::cout << chunkMaker.chunks[0]->toString() << "\n";
    std::cout << "All size:" << size << "\n";
    int N50=0;
    size_t lastSize=0;
    for(Chunk *c:chunkMaker.chunks){
        std::cout << c->size() << "\n";
        N50+=c->size();
        lastSize=c->size();
        if(N50>0.5*size){
            break;
        }
    }
    std::cout << "N50:" << lastSize << "\n";
    std::cout << "Number of chunks" << chunkMaker.chunks.size() << "\n";
    return 0;
}
