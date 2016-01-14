package io;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashSet;

import graph.Graph;
import overlap.Overlap;

public class GraphComparator {

	public static double compareGraph(Graph graph,String filePath,String solutionPath)throws IOException{
		HashSet<Overlap> overlaps=graph.getOverlaps();
		System.out.println("Overlaps size:"+overlaps.size());
		HashSet<Integer> indices=new HashSet<>();
		for(Overlap overlap:overlaps){
			indices.add(overlap.IDX);
		}
		//System.out.println(indices);
		ArrayList<String> myLines=GraphLoader.getLines(indices, filePath);
		ArrayList<String> solution=GraphLoader.getLines(solutionPath);
		int count=0;
		for(String s:myLines){
			if(solution.contains(s)){
				count++;
			}
		}
		//System.out.println(myLines);
		//System.out.println(solution);
		double eval=1.0*count/solution.size();
		System.out.println("Evaluation:"+eval);
		return eval;
	}
	
	public static void saveGraph(Graph graph,String filePath,String savePath) throws IOException{
		HashSet<Overlap> overlaps=graph.getOverlaps();
		System.out.println("Overlaps size:"+overlaps.size());
		HashSet<Integer> indices=new HashSet<>();
		for(Overlap overlap:overlaps){
			indices.add(overlap.IDX);
		}
		//System.out.println(indices);
		ArrayList<String> myLines=GraphLoader.getLines(indices, filePath);
		BufferedWriter writer=new BufferedWriter(new FileWriter(savePath));
		for(String line:myLines){
			writer.write(line);
			writer.write("\n");
		}
		writer.close();
	}
	
}
