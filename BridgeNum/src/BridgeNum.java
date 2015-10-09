import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Scanner;

public class BridgeNum {
	private ArrayList<Node> graph;
	private int vertexes, edges, bridges;
	ArrayDeque<Integer> deq;
	class Node {
		ArrayList<Integer> vrtxs;
		int parent;
		boolean b0, b1;
		Node() {
			parent = -1;
			b0 = b1 = false;
			vrtxs = new ArrayList<>();
		}
	}
	
	BridgeNum(Scanner in) {
		vertexes = in.nextInt();
		edges = in.nextInt();
		deq = new ArrayDeque<>();
		graph = new ArrayList<>();
		bridges = 0;
		for (int i = 0; i < vertexes; i++)
            		graph.add(new Node());
		for (int i = 0; i < edges; i++) {
			int foo = in.nextInt(), bar = in.nextInt();
			graph.get(foo).vrtxs.add(bar);
			graph.get(bar).vrtxs.add(foo);
		}
		for (int i = 0; i < vertexes; i++)
			if (!graph.get(i).b0) {
				DFSearch(i);
				bridges--;
			}
		while (!deq.isEmpty()) {
			int u = deq.poll();
			if (!graph.get(u).b1) {
				Visit(u);
				bridges++;
			}
		}
	}
	
	private void Visit(int i) {
		Node foo = graph.get(i);
		foo.b1 = true;
		for (int u : foo.vrtxs) {
			Node bar = graph.get(u);
			if (!bar.b1 && bar.parent != i)
				Visit(u);
		}
	}
	private void DFSearch(int i) {
		Node foo = graph.get(i);
		deq.add(i);
		foo.b0 = true;
		for (int u : foo.vrtxs) {
			Node bar = graph.get(u);
			if (!bar.b0) {
				bar.parent = i;
				DFSearch(u);
			}
		}
	}
	
	private void printBridges() {
		System.out.println(bridges);
	}
	
	public static void main(String[] args) {	
		BridgeNum res = new BridgeNum(new Scanner(System.in));
		res.printBridges();
	}
}
