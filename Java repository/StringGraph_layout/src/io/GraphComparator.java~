package io;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashSet;

import graph.Graph;
import overlap.Overlap;

public class GraphComparator {

	public static void compareGraph(Graph graph,String filePath,String solutionPath)throws IOException{
		HashSet<Overlap> overlaps=graph.getOverlaps();
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
		System.out.println("Evaluation:"+(1.0*count/solution.size()));
	}
	
}
