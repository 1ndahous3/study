import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map.Entry;
import java.util.Scanner;
import java.util.TreeSet;

public class EqDist {
	private final ArrayList<ArrayList<Integer>> graph = new ArrayList<>(0);
	private final HashMap<Integer, ArrayList<Integer>> support = new HashMap<>();
	private boolean[] vis;
	private int[] weight;
	private final int vertexes, edges;

	public static void main(String[] args) {
		EqDist res = new EqDist(new Scanner(System.in));
		res.printEq();
	}

	EqDist(Scanner in) {
		vertexes = in.nextInt();
		edges = in.nextInt();

		for (int i = 0; i < vertexes; i++) {
			graph.add(new ArrayList<>(0));
		}

		for (int i = 0; i < edges; i++) {
			int foo = in.nextInt(), bar = in.nextInt();
			graph.get(foo).add(bar);
			graph.get(bar).add(foo);
		}

		for (int i = in.nextInt(); i > 0; i--) {
			support.put(in.nextInt(), new ArrayList<>());
		}

		for (Entry<Integer, ArrayList<Integer>> obj : support.entrySet()) {
			Dijkstra(obj.getKey());
		}
	}

	private void Dijkstra(int n) {
		ArrayList<Integer> iter = new ArrayList<>(0);
		vis = new boolean[vertexes];
		weight = new int[vertexes];

		for (int i = 0; i < vertexes; i++) {
			weight[i] = -1;
		}
		weight[n] = 0;

		for (iter.add(n); !iter.isEmpty(); iter = nextVrtx(iter)) {
			iter.forEach(this::Vrtxs);
		}

		for (int i = 0; i < vertexes; i++)
			support.get(n).add(weight[i]);
	}

	private void Vrtxs(int n) {
		vis[n] = true;
		graph.get(n)
				.stream()
				.filter(i -> weight[i] == -1 || weight[n] + 1 < weight[i])
				.forEach(i -> weight[i] = weight[n] + 1);
	}

	private ArrayList<Integer> nextVrtx(ArrayList<Integer> foo) {
		ArrayList<Integer> res = new ArrayList<>(0);
		for (int bar : foo) {
			graph.get(bar)
					.stream()
					.filter(i -> !vis[i] && !res.contains(i))
					.forEach(res::add);
		}
		return res;
	}

	private void printEq() {
		TreeSet<Integer> tmp, res = new TreeSet<>();
		for (int i = 0; i < vertexes; i++) {
			tmp = new TreeSet<>();
			for (Entry<Integer, ArrayList<Integer>> foo : support.entrySet())
				tmp.add(foo.getValue().get(i));
			if (tmp.size() == 1 && (int)tmp.toArray()[0] != -1)
				res.add(i);
		}

		if (res.isEmpty()) {
			System.out.println("-");
		} else {
			res.forEach(System.out::println);
		}
	}
}