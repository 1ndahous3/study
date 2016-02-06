import java.util.Scanner;
import java.util.TreeMap;
import java.util.TreeSet;
import java.util.ArrayList;
import java.util.Map.Entry;

public class Dividers {
        private TreeMap<Long, TreeSet<Long>> graph;
	private ArrayList<Long> dividers;
	private long max;

	Dividers(long n) {
		max = n;
	}

	private void getGraph() {
		dividers = getDividers();
		graph = buildGraph();
	}

	public static void main(String[] args) {
		@SuppressWarnings("resource")
		Dividers res = new Dividers(new Scanner(System.in).nextLong());
		
		res.getGraph();
		res.printGraph();
	}

	private TreeMap<Long, TreeSet<Long>> buildGraph() {
		TreeMap<Long, TreeSet<Long>> graph_map = new TreeMap<>();
		
		for (int i = 0; i < dividers.size(); i++) {
			loop: for (int j = i + 1; j < dividers.size(); j++) {
				long foo = dividers.get(i), bar = dividers.get(j);
				if (bar % foo == 0) {
					for (int k = i + 1; k < j; k++) {
						long baz = dividers.get(k);
						if (bar % baz == 0 && baz % foo == 0) {
							continue loop;
						}
					}
					if (!graph_map.containsKey(bar))
						graph_map.put(bar, new TreeSet<Long>());
					graph_map.get(bar).add(foo);
				}
			}
		}
		
		return graph_map;
	}

	private void printGraph() {
		System.out.println("graph {");
		// print all dividers
		if (dividers.size() == 1 && max != 1)
			System.out.println("\t" + max);
		for (long i : dividers)
			System.out.println("\t" + i);
		// print all relations
		for (Entry<Long, TreeSet<Long>> foo : graph.entrySet())
			for (long bar : foo.getValue())
				System.out.println("\t" + foo.getKey() + " -- " + bar);
		if (graph.size() == 0 && max != 1)
			System.out.println("\t" + max + " -- 1");
		System.out.println("}");
	}

	private ArrayList<Long> getDividers() {
		TreeSet<Long> dividers_set = new TreeSet<>(); 
		
		for (long d = 1; d * d <= max; d++) {
			if (max % d == 0) {
				dividers_set.add(d);
				dividers_set.add(max / d);
			}
		}
		
		return new ArrayList<>(dividers_set);
	}
}