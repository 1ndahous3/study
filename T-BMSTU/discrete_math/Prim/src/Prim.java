import java.util.ArrayList;
import java.util.PriorityQueue;
import java.util.Scanner;

class Edge {
	final Node v;
	final int length;
	Edge(Node obj, int a) {
		v = obj;
		length = a;
	}
}

class Node {
	int key, index = -1;
	final ArrayList<Edge> edges = new ArrayList<>();
}

public class Prim {
	private int length;

	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		System.out.println(new Prim(in));
	}

	Prim(Scanner in) {
		length = 0;
		int vertexes = in.nextInt();
		int edges = in.nextInt();
		ArrayList<Node> vrtxs = new ArrayList<>();
		PriorityQueue<Node> queue = new PriorityQueue<>((a, b) -> Integer.compare(a.key, b.key));

		for (int i = 0; i < vertexes; i++) {
			vrtxs.add(new Node());
		}
		for (int i = 0; i < edges; i++) {
			int foo = in.nextInt(),
					bar = in.nextInt(),
					baz = in.nextInt();
			vrtxs.get(foo).edges.add(new Edge(vrtxs.get(bar), baz));
			vrtxs.get(bar).edges.add(new Edge(vrtxs.get(foo), baz));
		}
		queue.add(vrtxs.get(0));

		while (!queue.isEmpty()) {
			Node v = queue.poll();
			v.index = -2;
			for (Edge e : v.edges) {
				Node u = e.v;
				if (u.index == -1) {
					u.key = e.length;
					u.index = 1;
					queue.add(u);
				} else if (u.index != -2 && e.length < u.key) {
					queue.remove(u);
					u.key = e.length;
					queue.add(u);
				}
			}
			length += v.key;
		}
	}

	public String toString() {
		return Integer.toString(length);
	}
}