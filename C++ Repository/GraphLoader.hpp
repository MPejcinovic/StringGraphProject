
#pragma once
#include "Overlap.hpp"
#include <unordered_set>
#include <vector>
#include <set>
#include <map>
#include "Graph.hpp"
#include <fstream>
#include <iostream>

class GraphLoader {
    
private:

    
    
    std::vector<Overlap*> overlaps;
    std::map<int,Read*> reads;
    std::set<Read*>containedReads;
    
    OverlapPart* getOverlapPart(int ID,int fwd,int begin,int end,int originalBegin,int originalEnd,int length){
        auto it=reads.find(ID);
        Read *read=it->second;
        //System.out.println(read);
        if(it==reads.end()){
            read=new Read(ID, length);
            reads[ID]=read;
        }
        /*if(begin<length*0.005){
         begin=0;
         }
         if(end>length*0.995){
         end=length;
         }*/
        if(fwd==0){
            return new OverlapPart(read, begin, end,originalBegin,originalEnd);
        }
        return new OverlapPart(read, end, begin,originalEnd,originalBegin);
    }
    
    
public:
        
    Graph* getGraph(std::string filePath,std::string readsFilePath){
        std::ifstream file(filePath);
        std::string   line;
        overlaps.clear();
        reads.clear();
        containedReads.clear();

        int idx=0;
        while(std::getline(file, line)){
            std::stringstream   linestream(line);
            int ID1;
            linestream >>ID1;
            
            int ID2;;
            linestream >>ID2;

            
            linestream.ignore(128,' ');
            linestream.ignore(128,' ');
            linestream.ignore(128,' ');
            //linestream.ignore(128,' ');
            //linestream >>garbage;

            int fwd1;
            linestream >>fwd1;
            
            int begin1;
            linestream >>begin1;

            int end1;
            linestream >>end1;
            
            int length1;
            linestream >>length1;
            
            int originalBegin1=begin1;
            int originalEnd1=end1;
            
            int fwd2;
            linestream >>fwd2;
            
            int begin2;
            linestream >>begin2;
            
            int end2;
            linestream >>end2;
            
            int length2;
            linestream >>length2;
            
            int originalBegin2=begin2;
            int originalEnd2=end2;

            double bp1=1.0*begin1/length1;
            double ep1=1.0*(length1-(end1+1))/length1;
            
            double bp2=1.0*begin2/length2;
            double ep2=1.0*(length2-(end2+1))/length2;
            
            if(fwd2==0){
                if(bp1<bp2){
                    begin1=0;
                }else{
                    begin2=0;
                }
                if(ep1<ep2){
                    end1=length1-1;
                }else{
                    end2=length2-1;
                }
            }else{
                if(bp1<ep2){
                    begin1=0;
                }else{
                    end2=length2-1;
                }
                if(ep1<bp2){
                    end1=length1-1;
                }else{
                    begin2=0;
                }
            }
            
            OverlapPart *f=getOverlapPart(ID1,fwd1, begin1, end1,originalBegin1,originalEnd1, length1);
            OverlapPart *g=getOverlapPart(ID2,fwd2, begin2, end2,originalBegin2,originalEnd2, length2);
            
            Overlap *overlap=new Overlap(idx,f, g);
            if(f->isContainment()){
                containedReads.insert(f->getRead());
            }
            if(g->isContainment()){
                containedReads.insert(g->getRead());
            }
            overlaps.push_back(overlap);
            idx++;
            if(idx==25000){
                //break;
            }
        }
        
        file.close();
        std::ifstream readsFile(readsFilePath);
        int readID=1;
        while(std::getline(readsFile, line)){
            if(reads.find(readID)==reads.end()){
                std::getline(readsFile, line);
                std::getline(readsFile, line);//+
                std::getline(readsFile, line);//,+,+,+,+,
                readID++;
                continue;
            }
            Read *read=reads.find(readID)->second;
            if(containedReads.find(read)!=containedReads.end()){
                std::getline(readsFile, line);
                std::getline(readsFile, line);//+
                std::getline(readsFile, line);//,+,+,+,+,
                readID++;
                continue;
            }
            std::getline(readsFile, line);
            read->sequence=line;
            std::getline(readsFile, line);//+
            std::getline(readsFile, line);//,+,+,+,+,
            readID++;
        }
        
        readsFile.close();
        
        std::cout << "Num reads:" << reads.size() << "\n";
        std::cout << "Num overlaps:" << overlaps.size()<< "\n";
        
        /*  Finds the final reads that will be used to construct the graph - the reads that are not containments
         */
        auto readsIterator=reads.begin();
        while(readsIterator!=reads.end()){
            Read *read=readsIterator->second;
            if(containedReads.find(readsIterator->second)==containedReads.end()){
                ++readsIterator;
            }else{
                readsIterator=reads.erase(readsIterator);
                delete read;
            }
        }
        
        /*  Finds the final oerlaps that will be used to construct the graph - the overlaps that don't involve a contained read
         */
        for(int i=0;i<overlaps.size();i++){
            Overlap *overlap=overlaps[i];
            if(containedReads.find(overlap->f->getRead())!=containedReads.end()){
                overlaps[i]=NULL;
                delete overlap;
                continue;
            }
            if(containedReads.find(overlap->g->getRead())!=containedReads.end()){
                overlaps[i]=NULL;
                delete overlap;
                continue;
            }
        }
        return new Graph(reads, overlaps);
    }
};