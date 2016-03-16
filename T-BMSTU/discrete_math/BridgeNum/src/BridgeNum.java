import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Scanner;

public class BridgeNum {
	private final ArrayList<Node> graph = new ArrayList<>();
	private final int vertexes, edges;
	private int bridges = 0;
	private final ArrayDeque<Integer> deq = new ArrayDeque<>();

	class Node {
		final ArrayList<Integer> vrtxs = new ArrayList<>();
		int parent = -1;
		boolean b0 = false, b1 = false;
	}

	BridgeNum(Scanner in) {
		vertexes = in.nextInt();
		edges = in.nextInt();

		for (int i = 0; i < vertexes; i++) {
			graph.add(new Node());
		}

		for (int i = 0; i < edges; i++) {
			int foo = in.nextInt(), bar = in.nextInt();
			graph.get(foo).vrtxs.add(bar);
			graph.get(bar).vrtxs.add(foo);
		}

		for (int i = 0; i < vertexes; i++) {
			if (!graph.get(i).b0) {
				DFSearch(i);
				bridges--;
			}
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
		Node node = graph.get(i);
		node.b1 = true;
		for (int u : node.vrtxs) {
			Node sub_node = graph.get(u);
			if (!sub_node.b1 && sub_node.parent != i) {
				Visit(u);
			}
		}
	}

	private void DFSearch(int i) {
		Node node = graph.get(i);
		deq.add(i);
		node.b0 = true;
		for (int u : node.vrtxs) {
			Node sub_node = graph.get(u);
			if (!sub_node.b0) {
				sub_node.parent = i;
				DFSearch(u);
			}
		}
	}

	public String toString() {
		return Integer.toString(bridges);
	}

	public static void main(String[] args) {
		BridgeNum res = new BridgeNum(new Scanner(System.in));
		System.out.println(res);
	}
}