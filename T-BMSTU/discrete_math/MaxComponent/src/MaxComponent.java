import java.util.*;
import java.util.AbstractMap.SimpleEntry;

public class MaxComponent {
	private final TreeMap<Integer, ArrayList<Integer>> edges = new TreeMap<>();
	private ArrayList<SimpleEntry<Integer, TreeSet<Integer>>> comps;

	public MaxComponent(TreeMap<Integer, ArrayList<Integer>> _edges) {
		edges.putAll(_edges);
		splitComponents();
	}

	public String toString() {
		@SuppressWarnings("unchecked")
		SimpleEntry<Integer, TreeSet<Integer>> red =
		(SimpleEntry<Integer, TreeSet<Integer>>)comps
				.stream()
				.sorted((SimpleEntry<Integer, TreeSet<Integer>> a, SimpleEntry<Integer, TreeSet<Integer>> b) ->
						a.getValue().size() != b.getValue().size() ?
								b.getValue().size() - a.getValue().size() : a.getKey() != b.getKey() ?
								b.getKey() - a.getKey() : a.getValue().first() - b.getValue().first())
				.toArray()[0];

		StringBuilder res = new StringBuilder("graph {\n");

		for (Map.Entry<Integer, ArrayList<Integer>> obj : edges.entrySet()) {
			int num = obj.getKey();
			res.append("\t").append(num);
			if (red.getValue().contains(num)) {
				res.append(" [color = red]");
			}
			res.append("\n");
		}

		for (Map.Entry<Integer, ArrayList<Integer>> obj : edges.entrySet()) {
			int num1 = obj.getKey();
			obj.getValue()
					.stream()
					.filter(num2 -> num2 <= num1)
					.forEach(num2 -> {
						res.append("\t").append(num2).append(" -- ").append(num1);
						if (red.getValue().contains(num1)) {
							res.append(" [color = red]");
						}
						res.append("\n");
					});
		}
		res.append("}");

		return res.toString();
	}

	private void splitComponents() {
		comps = new ArrayList<>();
		ArrayDeque<Integer> queue = new ArrayDeque<>();
		TreeSet<Integer> set;
		int br_counter;
		boolean[] visited = new boolean[edges.size()];

		for (int i = 0; i < edges.size(); i++) {
			if (visited[i]) {
				continue;
			}
			set = new TreeSet<>();
			set.add(i);
			br_counter = checkVrtx(queue, visited, i);
			while (!queue.isEmpty()) {
				int foo = queue.poll();
				set.add(foo);
				br_counter += checkVrtx(queue, visited, foo);
				visited[foo] = true;
			}
			comps.add(new SimpleEntry<>(br_counter, set));
			visited[i] = true;
		}
	}

	private int checkVrtx(ArrayDeque<Integer> queue, boolean[] visited, int j) {
		int bridges_count = 0;
		for (int i: edges.get(j)) {
			if (!visited[i]) {
				queue.add(i);
				bridges_count++;
			}
		}

		return bridges_count;
	}

	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		int n_of_vrtxs = in.nextInt(), n_of_bridges = in.nextInt();
		TreeMap<Integer, ArrayList<Integer>> edges = new TreeMap<>();

		for (int i = 0; i < n_of_vrtxs; i++) {
			edges.put(i, new ArrayList<>());
		}

		for (int i = 0; i < n_of_bridges; i++) {
			int foo = in.nextInt(), bar = in.nextInt();
			edges.get(foo).add(bar);
			if (foo != bar) {
				edges.get(bar).add(foo);
			}
		}

		MaxComponent comp = new MaxComponent(edges);

		System.out.println(comp);
	}
}