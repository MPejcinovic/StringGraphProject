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
	
	private OverlapPart getOverlapPart(int ID,int fwd,int begin,int end,int length){
		Read read=reads.get(ID);
		if(read==null){
			read=new Read(ID, length);
			reads.put(ID, read);
		}
		if(fwd==0){
			return new OverlapPart(read, begin, end);
		}
		return new OverlapPart(read, end, begin);
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
	
	public Graph getGraph(String filePath) throws IOException{
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

			int fwd2=Integer.parseInt(arr[8]);
			int begin2=Integer.parseInt(arr[9]);
			int end2=Integer.parseInt(arr[10]);
			int length2=Integer.parseInt(arr[11]);

			OverlapPart f=getOverlapPart(ID1,fwd1, begin1, end1, length1);
			OverlapPart g=getOverlapPart(ID2,fwd2, begin2, end2, length2);
			
			Overlap overlap=new Overlap(idx,f, g);
			Read containedRead=overlap.getContainedRead();
			if(containedRead!=null){
				containedReads.add(containedRead);
			}
			overlaps.add(overlap);
			idx++;
		}
		reader.close();
		
		System.out.println("Num reads:"+reads.size());
		
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
		
		return new Graph(finalReads, finalOverlaps);
	}
	
}
