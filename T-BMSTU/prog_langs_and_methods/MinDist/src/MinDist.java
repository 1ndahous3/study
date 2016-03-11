import java.util.Scanner;

public class MinDist {
	public static int getDiff(String S, char x, char y) {
		int a = Integer.MAX_VALUE, b = Integer.MAX_VALUE, diff = Integer.MAX_VALUE;

		for (int i = 0; i < S.length(); i++) {
			if (S.charAt(i) == x) {
				a = i;
				diff = Math.min(Math.abs(a - b), diff);
			} else if (S.charAt(i) == y) {
				b = i;
				diff = Math.min(Math.abs(a - b), diff);
			}
		}

		return diff - 1;
	}

	@SuppressWarnings("resource")
	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		String s = in.nextLine();
		char x = in.next().charAt(0), y = in.next().charAt(0);
		System.out.println(getDiff(s, x, y));
	}
}