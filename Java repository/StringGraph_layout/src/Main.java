import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;

import chunk.Chunk;
import chunk.ChunkMaker;
import graph.Graph;
import graph.operations.InternalVertexRemover;
import graph.operations.NetworkFlowAnalyzer;
import graph.operations.TransitiveEdgeRemover;
import io.GraphComparator;
import io.GraphLoader;
import overlap.Read;

public class Main {

	private static final String filePath="overlaps.mhap";
	private static final String solutionPath="overlaps.after_unitigger.mhap";
	private static final String savePath="output";
	private static final String readsFileName="reads.fq";
	
	public static void main(String[] args) throws IOException {
		GraphLoader loader=new GraphLoader();
		
		Graph graph=loader.getGraph(filePath,readsFileName);
		
		System.out.println("After containment:"+graph);
		GraphComparator.compareGraph(graph, filePath, solutionPath);
		
		TransitiveEdgeRemover transitiveEdgeRemover=new TransitiveEdgeRemover(graph);
		transitiveEdgeRemover.process();
		System.out.println("After transitive:"+graph);
		graph.removeEmptyVertices();
		System.out.println("After removing empty:"+graph);
		GraphComparator.compareGraph(graph, filePath, solutionPath);

		InternalVertexRemover internalVertexRemover=new InternalVertexRemover(graph);
		//internalVertexRemover.process();
		System.out.println("After internalVertexRemover:"+graph);
		graph.removeEmptyVertices();
		System.out.println("After removing empty:"+graph);
		GraphComparator.compareGraph(graph, filePath, solutionPath);


		NetworkFlowAnalyzer networkFlowAnalyzer=new NetworkFlowAnalyzer(graph);
		//networkFlowAnalyzer.process();
		System.out.println(graph);
		graph.removeEmptyVertices();
		System.out.println(graph);
				

		ChunkMaker chunkMaker=new ChunkMaker(graph);
		chunkMaker.process();

		int s=0;
		for(Chunk chunk:chunkMaker.chunks){
			s+=chunk.size();
			System.out.println(chunk.size());
		}
		BufferedWriter writer=new BufferedWriter(new FileWriter("chunks.gfa"));
		int[] arr=new int[]{3,-2,10,-35,31,19,34,-20,14,-60,-49,6,8,-4,17,-7,-0,11,25,-18,-23,-16,-42,30,-40,39,32};
		System.out.println("LEN"+arr.length);
		for(int i=0;i<arr.length;i++){
			int idx=arr[i];		
			int offset=0;
			Chunk cr=chunkMaker.chunks.get(Math.abs(idx)+offset);
			cr.idx=idx+offset;
			StringBuilder GFA=new StringBuilder(cr.toGFA60());
			if(idx<0||idx==0){
				GFA.reverse();
				GFA=new StringBuilder(Read.complement(GFA.toString()));
			}
			GFA.append("\n");
			writer.write("S\t"+(idx+offset)+"\t"+GFA.toString());
		}
		writer.close();
		String GFA=graph.toGFA();
		writer=new BufferedWriter(new FileWriter("graph.gfa"));
		writer.write(GFA);
		writer.close();
		//System.out.println(graph.toGFA());
		System.out.println(graph);

		System.out.println(s);
		System.out.println("Number of chunks:"+chunkMaker.chunks.size());
		double eval=GraphComparator.compareGraph(graph, filePath, solutionPath);
		GraphComparator.saveGraph(graph, filePath, savePath+"-"+eval+".mhap");
	}

}

