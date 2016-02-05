import java.util.HashSet;
import java.util.Scanner;

public class Econom {
	private static int shrink_expr(String line) {
		int index_curr, index_end = 0;
		HashSet<String> hset = new HashSet<String>();

		while ((index_curr = line.indexOf(')', index_end)) != -1) {
			int index_start = index_curr - 1;

			for (int i = 1; i != 0; index_start--) {
				char sym = line.charAt(index_start);
				if (sym == ')')
					i++;
				else if (sym == '(')
					i--;
			}

			index_start++;
			hset.add(line.substring(index_start, index_curr));
			index_end = index_curr + 1;
		}
		return hset.size();
	}

	@SuppressWarnings("resource")
	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		System.out.println(Econom.shrink_expr(in.nextLine()));
	}
}