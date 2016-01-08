import java.io.IOException;
import graph.Graph;
import io.GraphComparator;
import io.GraphLoader;
import graph.operations.InternalVertexRemover;
import graph.operations.NetworkFlowAnalyzer;
import graph.operations.TransitiveEdgeRemover;

public class Main {

	/**
	 * @param args
	 */
	private static final String filePath="overlaps.mhap";
	private static final String solutionPath="overlaps.after_unitigger.mhap";
	
	public static void main(String[] args) throws IOException {
		
		GraphLoader loader=new GraphLoader();
		Graph graph=loader.getGraph(filePath);
		System.out.println(graph);
		System.out.println(graph);
		GraphComparator.compareGraph(graph, filePath, solutionPath);

		TransitiveEdgeRemover transitiveEdgeRemover=new TransitiveEdgeRemover(graph);
		transitiveEdgeRemover.process();
		System.out.println(graph);
		graph.removeEmptyVertices();
		System.out.println(graph);
		GraphComparator.compareGraph(graph, filePath, solutionPath);

		InternalVertexRemover internalVertexRemover=new InternalVertexRemover(graph);
		internalVertexRemover.process();
		System.out.println(graph);
		graph.removeEmptyVertices();
		System.out.println(graph);
		GraphComparator.compareGraph(graph, filePath, solutionPath);

		NetworkFlowAnalyzer networkFlowAnalyzer=new NetworkFlowAnalyzer(graph);
		networkFlowAnalyzer.process();
		System.out.println(graph);
		graph.removeEmptyVertices();
		System.out.println(graph);
		
		GraphComparator.compareGraph(graph, filePath, solutionPath);
	}

}
