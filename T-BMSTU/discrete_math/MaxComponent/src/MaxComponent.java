import java.util.*;
import java.util.AbstractMap.SimpleEntry;

public class MaxComponent {
	private final TreeMap<Integer, TreeSet<Integer>> bridges = new TreeMap<>();
	private ArrayList<SimpleEntry<Integer, TreeSet<Integer>>> comps;

	public MaxComponent(TreeMap<Integer, TreeSet<Integer>> _bridges) {
		bridges.putAll(_bridges);
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
								b.getKey() - a.getKey() : (Integer)(a.getValue().toArray()[0]) -  (Integer)(b.getValue().toArray()[0]))
			.toArray()[0];

		StringBuilder res = new StringBuilder("graph {\n");

		for (Map.Entry<Integer, TreeSet<Integer>> obj : bridges.entrySet()) {
			int num = obj.getKey();
			res.append("\t").append(num);
			if (red.getValue().contains(num)) {
				res.append(" [color = red]");
			}
			res.append("\n");
		}

		for (Map.Entry<Integer, TreeSet<Integer>> obj : bridges.entrySet()) {
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
		PriorityQueue<Integer> queue = new PriorityQueue<>();
		int br_counter = 0;
		TreeSet<Integer> set = new TreeSet<>();
		set.add(0);

		boolean[] visited = new boolean[bridges.size()];
		for (int i = 0; i < bridges.size(); i++) {
			visited[i] = false;
		}

		for (int i = 0; i < bridges.size();) {
			if (!queue.isEmpty()) {
				int foo = queue.poll();
				set.add(foo);
				br_counter += checkVrtx(queue, visited, foo);
				visited[foo] = true;
				continue;
			}
			comps.add(new SimpleEntry<>(br_counter, set));

			set = new TreeSet<>();
			set.add(i);
			br_counter = checkVrtx(queue, visited, i);
			visited[i++] = true;
		}
	}

	private int checkVrtx(PriorityQueue<Integer> queue, boolean[] visited, int j) {
		int bridges_count = 0;
		for (int i: bridges.get(j)) {
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
		TreeMap<Integer, TreeSet<Integer>> bridges = new TreeMap<>();

		for (int i = 0; i < n_of_vrtxs; i++) {
			bridges.put(i, new TreeSet<>());
		}

		for (int i = 0; i < n_of_bridges; i++) {
			int foo = in.nextInt(), bar = in.nextInt();
			bridges.get(foo).add(bar);
			bridges.get(bar).add(foo);
		}

		MaxComponent comp = new MaxComponent(bridges);
		System.out.println(comp);
	}
}
