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
	private static ChunkMaker chunkMaker;
	
	
	private static void write60() throws IOException{
		BufferedWriter writer=new BufferedWriter(new FileWriter("chunks60.gfa"));
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
	}

	private static void write45() throws IOException{
		BufferedWriter writer=new BufferedWriter(new FileWriter("chunks45.gfa"));
		//int[] arr=new int[]{3,-2,10,-35,31,19,34,-20,14,-60,-49,6,8,-4,17,-7,-0,11,25,-18,-23,-16,-42,30,-40,39,32};
		int[] arr=new int[]{3,-2,9,4,36,-34,22,10,-20,39,-25,17,11,6,-5,7,30,-12,-1,15,-23,-32,-21,24,-8,-19,-25,44,38,42};

		//int[] arr=new int[]{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14};
		System.out.println("LEN"+arr.length);
		for(int i=0;i<arr.length;i++){
			int idx=arr[i];		
			int offset=0;
			Chunk cr=chunkMaker.chunks.get(Math.abs(idx)+offset);
			cr.idx=idx+offset;
			StringBuilder GFA=new StringBuilder(cr.toGFA());
			if(idx<0||idx==0){
				GFA.reverse();
				GFA=new StringBuilder(Read.complement(GFA.toString()));
			}
			GFA.append("\n");
			writer.write("S\t"+(idx+offset)+"\t"+GFA.toString());
		}
		writer.close();
	}

	public static void main(String[] args) throws IOException {
		GraphLoader loader=new GraphLoader();
		
		Graph graph=loader.getGraph(filePath,readsFileName);
		System.out.println("Memory:"+(Runtime.getRuntime().totalMemory()/(1024*1024)));

		System.out.println("After containment:"+graph);
		GraphComparator.compareGraph(graph, filePath, solutionPath);
		
		TransitiveEdgeRemover transitiveEdgeRemover=new TransitiveEdgeRemover(graph);
		transitiveEdgeRemover.process();
		System.out.println("After transitive:"+graph);
		graph.removeEmptyVertices();
		System.out.println("After removing empty:"+graph);
		GraphComparator.compareGraph(graph, filePath, solutionPath);

		System.out.println("Memory:"+(Runtime.getRuntime().totalMemory()/(1024*1024)));
		
		InternalVertexRemover internalVertexRemover=new InternalVertexRemover(graph);
		//internalVertexRemover.process();
		System.out.println("After internalVertexRemover:"+graph);
		graph.removeEmptyVertices();
		System.out.println("After removing empty:"+graph);
		GraphComparator.compareGraph(graph, filePath, solutionPath);

		System.out.println("Memory:"+(Runtime.getRuntime().totalMemory()/(1024*1024)));

		NetworkFlowAnalyzer networkFlowAnalyzer=new NetworkFlowAnalyzer(graph);
		//networkFlowAnalyzer.process();
		System.out.println(graph);
		graph.removeEmptyVertices();
		System.out.println(graph);
				

		chunkMaker=new ChunkMaker(graph);
		chunkMaker.process();
		write45();
		for(Chunk c:chunkMaker.chunks){
			System.out.println(c.size());
		}
		System.out.println("Memory:"+(Runtime.getRuntime().totalMemory()/(1024*1024)));

		System.out.println("Number of chunks:"+chunkMaker.chunks.size());
		double eval=GraphComparator.compareGraph(graph, filePath, solutionPath);
		GraphComparator.saveGraph(graph, filePath, savePath+"-"+eval+".mhap");
	}

}
