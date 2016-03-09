import java.util.AbstractMap.SimpleEntry;
import java.util.ArrayList;
import java.util.PriorityQueue;
import java.util.Scanner;
import java.util.TreeSet;

public class Mars {
	private final ArrayList<Integer> free_persons;
	private TreeSet<Integer> team_one, team_two;
	private final PriorityQueue<Integer> queue;
	private final boolean[][] mtrx;
	private final boolean[] checked;
	private final int n;

	public Mars(int nel, boolean _mtrx[][]) {
		n = nel;
		mtrx = _mtrx;
		checked = new boolean[nel];
		team_one = new TreeSet<>();
		team_two = new TreeSet<>();
		queue = new PriorityQueue<>();
		free_persons = new ArrayList<>();
		for (int i = 0; i < n; i++) {
			free_persons.add(i);
		}
	}

	private void addToGroup(TreeSet<Integer> group_foo, int i) {
		group_foo.add(i);
		if (free_persons.contains(i)) {
			free_persons.remove((Integer) i);
		}
	}

	private boolean pushPerson(TreeSet<Integer> group_foo, TreeSet<Integer> group_bar, int i) {
		if (group_foo.contains(i)) {
			return false;
		} else if (!checked[i]) {
			addToGroup(group_bar, i);
			if (!queue.contains(i)) {
				queue.add(i);
			}
		}

		return true;
	}

	private boolean splitTeams(TreeSet<Integer> group_one, TreeSet<Integer> group_two, int i, int j) {
		if (group_one.contains(i)) {
			return pushPerson(group_one, group_two, j);
		} else if (group_two.contains(i)) {
			return pushPerson(group_two, group_one, j);
		} else {
			addToGroup(group_one, i);
			addToGroup(group_two, j);
			if (!checked[j]) {
				queue.add(j);
			}
		}

		return true;
	}

	private void combine(ArrayList<SimpleEntry<TreeSet<Integer>, TreeSet<Integer>>> groups) {
		TreeSet<Integer> group_min = getComb(groups, 0), group_max = getComb(groups, ~0), group_a, group_b;
		pushFreePersons(group_min, group_max, free_persons);

		int diff = Math.abs(group_min.size() - group_max.size());

		for (long i = 1; i < Math.pow(2, groups.size()); i++) {
			group_a = getComb(groups, i);
			group_b = getComb(groups, ~i);

			pushFreePersons(group_a, group_b, free_persons);

			int diff_curr = Math.abs(group_a.size() - group_b.size());
			if (diff_curr < diff) {
				diff = diff_curr;
				group_min = group_a;
				group_max = group_b;
			} else if (diff_curr == diff) {
				if (compareCombs(group_min, group_a)) {
					group_min = group_a;
					group_max = group_b;
				}
				if (compareCombs(group_min, group_b)) {
					group_min = group_b;
					group_max = group_a;
				}
			}
		}

		team_one = group_min;
		team_two = group_max;
	}

	private boolean checkPerson(TreeSet<Integer> group_one, TreeSet<Integer> group_two, int i) {
		for (int j = 0; j < n; j++) {
			if (mtrx[i][j]) {
				if (!splitTeams(group_one, group_two, i, j)) {
					return false;
				}
			}
		}

		return checked[i] = true;
	}

	private static TreeSet<Integer> getComb(ArrayList<SimpleEntry<TreeSet<Integer>, TreeSet<Integer>>> groups, long k) {
		TreeSet<Integer> group = new TreeSet<>();

		for (int i = 0; i < groups.size(); i++) {
			group.addAll((k & (long) Math.pow(2, i)) == 0 ? groups.get(i).getKey() : groups.get(i).getValue());
		}

		return group;
	}

	private static boolean compareCombs(TreeSet<Integer> group_a, TreeSet<Integer> group_b) {
		if (group_a.size() != group_b.size()) {
			return group_a.size() > group_b.size();
		}

		for (int i = 0; i < group_a.size(); i++) {
			int a = (int) group_a.toArray()[i], b = (int) group_b.toArray()[i];
			if (a != b) {
				return a > b;
			}
		}

		return false;
	}

	private ArrayList<SimpleEntry<TreeSet<Integer>, TreeSet<Integer>>> getBipartiteGraph() {
		ArrayList<SimpleEntry<TreeSet<Integer>, TreeSet<Integer>>> groups = new ArrayList<>();
		TreeSet<Integer> group_one = new TreeSet<>(), group_two = new TreeSet<>();

		for (int i = 0; i < n;) {
			if (!queue.isEmpty()) {
				if (!checkPerson(group_one, group_two, queue.remove())) {
					return null;
				}
				continue;
			} else if (!checked[i] && !checkPerson(group_one, group_two, i)) {
				return null;
			}
			if (queue.isEmpty() && !group_one.isEmpty()) {
				groups.add(new SimpleEntry<>(group_one, group_two));
				group_one = new TreeSet<>();
				group_two = new TreeSet<>();
			}
			i++;
		}

		return groups;
	}

	private static void pushFreePersons(TreeSet<Integer> group_one, TreeSet<Integer> group_two,
										ArrayList<Integer> free) {
		ArrayList<Integer> free_loc = new ArrayList<>(free);
		int n_one = 0, free_n = free_loc.size(), diff = group_two.size() - group_one.size();
		if (diff < 0) {
			for (; diff < 0 && free_n > 0; diff++, free_n--);
		} else {
			for (; diff > 0 && free_n > 0; diff--, free_n--, n_one++);
		}
		n_one += free_n / 2;

		for (int i = 0; i < n_one; i++) {
			group_one.add(free_loc.remove(0));
		}
		group_two.addAll(free_loc);
	}

	private boolean getTeams() {
		ArrayList<SimpleEntry<TreeSet<Integer>, TreeSet<Integer>>> independent_groups = getBipartiteGraph();
		if (independent_groups == null) {
			return false;
		}

		combine(independent_groups);

		return true;
	}

	public static void main(String[] args) {
		@SuppressWarnings("resource")
		Scanner in = new Scanner(System.in);
		int nel = in.nextInt();
		boolean mtrx[][] = new boolean[nel][nel];

		for (int i = 0; i < nel; i++) {
			for (int j = 0; j < nel; j++) {
				mtrx[i][j] = !in.next().equals("-");
			}
		}

		Mars res = new Mars(nel, mtrx);

		if (res.getTeams()) {
			res.team_one.forEach(i -> System.out.println((i + 1) + " "));
		} else {
			System.out.println("No solution");
		}
	}
}
