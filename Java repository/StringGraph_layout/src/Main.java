import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashSet;

import chunk.Chunk;
import chunk.ChunkMaker;
import graph.Edge;
import graph.Graph;
import graph.Vertex;
import graph.operations.InternalVertexRemover;
import graph.operations.NetworkFlowAnalyzer;
import graph.operations.TransitiveEdgeRemover;
import io.GraphLoader;
import overlap.Read;

public class Main {

	private static final String filePath="overlaps.mhap";
	private static final String solutionPath="overlaps.after_unitigger.mhap";
	private static final String savePath="output";
	private static final String readsFileName="reads.fq";
	private static ChunkMaker chunkMaker;
	private static Graph graph;
	
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
		//int[] arr=new int[]{3,-2,9,4,36,-34,22,10,-20,39,-25,17,11,6,-5,7,30,-12,-1,15,-23,-32,-21,24,-8,-19,-25,44,38,42};

		int[] arr=new int[]{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14};
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

	private static void write45All() throws IOException{
		BufferedWriter writer=new BufferedWriter(new FileWriter("chunks45.gfa"));
		for(int i=0;i<chunkMaker.chunks.size();i++){
			Chunk cr=chunkMaker.chunks.get(i);
			cr.idx=i;
			if(cr.edges.size()!=0){
				StringBuilder GFA=new StringBuilder(cr.toGFA());
				GFA.append("\n");
				writer.write("S\t"+(i)+"\t"+GFA.toString());
			}
		}
		writer.close();
	}

	private static void test() throws IOException{
		BufferedWriter writer=new BufferedWriter(new FileWriter("chunks45.gfa"));
		int[] chunks=new int[]{0,12,-1,-8,-5,1,2,-0,-4,1,-7,9,-3,-6,11,-2,-1,7,-10,-5,14,0};
		int[] begins=new int[]{543250,0,0,0,0,300000,300000,1260340,0,75000,160000,0,0,0,0,0,660000,0,0,110000,0,0};
		int[] ends=new int[]{1262070,0,100000,0,100000,700000,0,0,0,300000,0,0,0,0,0,315000,0,150000,0,0,0,543250};
		
		for(int i=0;i<chunks.length;i++){
			int idx=chunks[i];
			Chunk cr=chunkMaker.chunks.get(Math.abs(idx));
			cr.idx=i;
			int begin=begins[i];
			int end=ends[i];
			if(end==0){
				end=cr.size();
			}
			StringBuilder GFA=new StringBuilder(cr.toGFA());
			String s=null;
			if(i==0||idx>0||i==chunks.length-1){
				s=GFA.substring(begin, end);
			}else{
				StringBuilder b=new StringBuilder(Read.complement(GFA.substring(begin, end)));
				b.reverse();
				s=b.toString();
			}
			GFA=new StringBuilder(s);
			GFA.append("\n");
			writer.write("S\t"+(i)+"\t"+GFA.toString());
		}
		writer.close();
	}

	private static void writeBandageChunks() throws Exception{
		BufferedWriter w=new BufferedWriter(new FileWriter("bandage-chunks.gfa"));
		w.write("H\tVN:Z:1.0\n");
		HashSet<Read> reads=new HashSet<>();
		for(Chunk c:chunkMaker.chunks){
			for(Vertex v:c.vertices){
				if(!reads.contains(v.getRead())){
					w.write(v.getRead().toGFA());
					w.write("\n");
				}
				reads.add(v.getRead());
			}
			for(Edge e:c.edges){
				w.write(e.toGFARecursive());
				w.write("\n");
			}
		}
		w.close();
	}

	private static void writeBandageGraph() throws Exception{
		BufferedWriter w=new BufferedWriter(new FileWriter("bandage-graph.gfa"));
		w.write(graph.toGFA());
		w.close();
	}
	
	public static void main(String[] args) throws Exception {
		GraphLoader loader=new GraphLoader();
		System.out.println("Memory:"+(Runtime.getRuntime().totalMemory()/(1024*1024)));

		graph=loader.getGraph(filePath,readsFileName);
		System.out.println("Memory:"+(Runtime.getRuntime().totalMemory()/(1024*1024)));

		System.out.println("After containment:"+graph);
		
		TransitiveEdgeRemover transitiveEdgeRemover=new TransitiveEdgeRemover(graph);
		transitiveEdgeRemover.process();
		System.out.println("After transitive:"+graph);
		//System.exit(0);
		graph.removeEmptyVertices();
		System.out.println("After removing empty:"+graph);

		System.out.println("Memory:"+(Runtime.getRuntime().totalMemory()/(1024*1024)));
		
		InternalVertexRemover internalVertexRemover=new InternalVertexRemover(graph);
		internalVertexRemover.process();
		System.out.println("After internalVertexRemover:"+graph);
		graph.removeEmptyVertices();
		System.out.println("After removing empty:"+graph);

		System.out.println("Memory:"+(Runtime.getRuntime().totalMemory()/(1024*1024)));

		NetworkFlowAnalyzer networkFlowAnalyzer=new NetworkFlowAnalyzer(graph);
		networkFlowAnalyzer.process();
		System.out.println(graph);
		graph.removeEmptyVertices();
		System.out.println(graph);
				

		chunkMaker=new ChunkMaker(graph);
		chunkMaker.process();
		write45All();
		writeBandageChunks();
		writeBandageGraph();
		int size=0;
		int a=0;
		for(Chunk c:chunkMaker.chunks){
			//System.out.println(c.size());
			size+=c.size();
			if(c.edges.size()==0){
				a++;
			}
		}
		System.out.println("Ove vertex chunks:"+a);
		System.out.println(chunkMaker.chunks.get(0));
		System.out.println("All size:"+size);
		int N50=0;
		int lastSize=0;
		for(Chunk c:chunkMaker.chunks){
			System.out.println(c.size());
			N50+=c.size();
			lastSize=c.size();
			if(N50>0.5*size){
				break;
			}
		}
		System.out.println("N50:"+lastSize);
		System.out.println("Memory:"+(Runtime.getRuntime().totalMemory()/(1024*1024)));

		System.out.println("Number of chunks:"+chunkMaker.chunks.size());
	}

}
