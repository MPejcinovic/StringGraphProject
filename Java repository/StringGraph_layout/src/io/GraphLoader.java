package io;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;

import graph.Graph;
import overlap.Overlap;
import overlap.OverlapPart;
import overlap.Read;

public class GraphLoader {

	private ArrayList<Overlap> overlaps;
	private HashMap<Integer,Read> reads;
	private HashSet<Read> containedReads;
	
	// get overlap; find out the direction anf build overlapPart
	private OverlapPart getOverlapPart(int ID,int fwd,int begin,int end,int originalBegin,int originalEnd,int length){
		Read read=reads.get(ID);
		//System.out.println(read);
		if(read==null){
			read=new Read(ID, length);
			reads.put(ID, read);
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
	
	public static ArrayList<String> getLines(HashSet<Integer> indices,String filePath)throws IOException{
		BufferedReader reader=new BufferedReader(new FileReader(filePath));
		String line;
		ArrayList<String> arr=new ArrayList<>();
		int idx=0;
		while((line=reader.readLine())!=null){
			if(indices.contains(idx)){
				arr.add(line);
			}
			idx++;
		}
		reader.close();
		return arr;
	}

	public static ArrayList<String> getLines(String filePath)throws IOException{
		BufferedReader reader=new BufferedReader(new FileReader(filePath));
		String line;
		ArrayList<String> arr=new ArrayList<>();
		while((line=reader.readLine())!=null){
			arr.add(line);
		}
		reader.close();
		return arr;
	}
	
	//read file and find out whether to compare begins and ends, or one begin with another end vice versa; get rid of containments
	public Graph getGraph(String filePath,String readsFilePath) throws IOException{
		BufferedReader reader=new BufferedReader(new FileReader(filePath));
		String line;
		overlaps=new ArrayList<>();
		reads=new HashMap<>();
		containedReads=new HashSet<>();
		int idx=0;
		while((line=reader.readLine())!=null){
			String[] arr=line.split(" ");
			int ID1=Integer.parseInt(arr[0]);
			int ID2=Integer.parseInt(arr[1]);
			
			int fwd1=Integer.parseInt(arr[4]);
			int begin1=Integer.parseInt(arr[5]);
			int end1=Integer.parseInt(arr[6]);
			int length1=Integer.parseInt(arr[7]);
			int originalBegin1=begin1;
			int originalEnd1=end1;

			int fwd2=Integer.parseInt(arr[8]);
			int begin2=Integer.parseInt(arr[9]);
			int end2=Integer.parseInt(arr[10]);
			int length2=Integer.parseInt(arr[11]);
			int originalBegin2=begin2;
			int originalEnd2=end2;

			/*System.out.println(idx+" "+ID1+" "+ID2+
					" "+fwd1+" "+begin1+" "+end1+" "+length1+ 
					" "+fwd2+" "+begin2+" "+end2+" "+length2);*/
			
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
			
			OverlapPart f=getOverlapPart(ID1,fwd1, begin1, end1,originalBegin1,originalEnd1, length1);
			OverlapPart g=getOverlapPart(ID2,fwd2, begin2, end2,originalBegin2,originalEnd2, length2);
			
			Overlap overlap=new Overlap(idx,f, g);
			if(f.isContainment()){
				containedReads.add(f.getRead());
			}
			if(g.isContainment()){
				containedReads.add(g.getRead());
			}
			overlaps.add(overlap);
			idx++;
		}
		reader.close();
		
		reader=new BufferedReader(new FileReader(readsFilePath));
		int readID=1;
		while((line=reader.readLine())!=null){//ID
			Read read=reads.get(readID);
			if(read==null){
				reader.readLine();
				reader.readLine();//+
				reader.readLine();//,+,+,+,+,
				readID++;
				continue;
			}
			read.sequence=reader.readLine();
			reader.readLine();//+
			reader.readLine();//,+,+,+,+,
			readID++;
		}
		
		reader.close();
		
		System.out.println("Num reads:"+reads.size());
		System.out.println("Num overlaps:"+overlaps.size());
		
		ArrayList<Read> finalReads=new ArrayList<>();
		for(Read read:reads.values()){
			if(!containedReads.contains(read)){
				finalReads.add(read);
			}else{
				//System.out.println("skipped read:"+read);
			}
		}
		
		ArrayList<Overlap> finalOverlaps=new ArrayList<>();
		
		for(Overlap overlap:overlaps){
			if(containedReads.contains(overlap.f.getRead())||containedReads.contains(overlap.g.getRead())){
				//System.out.println("skipped overlap:"+overlap);
				continue;
			}
			finalOverlaps.add(overlap);
		}
		System.out.println("Num finalReads:"+finalReads.size());
		System.out.println("Num finalOverlaps:"+finalOverlaps.size());

		return new Graph(finalReads, finalOverlaps);
	}
	
}
