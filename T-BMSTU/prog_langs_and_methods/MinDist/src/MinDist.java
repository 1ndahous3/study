import java.util.Scanner;

class StringLoop {
	public static int getDiff(String S, char x, char y) {
		int a = 0x7fffffff, b = 0x7fffffff, diff = 0x7fffffff;
		
		for (int i = 0; i < S.length(); i++) {
			if (S.charAt(i) == x) {
				a = i;
				if (Math.abs(a - b) < diff)
					diff = Math.abs(a - b);
			}
			if (S.charAt(i) == y) {
				b = i;
				if (Math.abs(a - b) < diff)
					diff = Math.abs(a - b);
			}
		}
		
		return diff - 1;
	}
}

public class MinDist {
	@SuppressWarnings("resource")
	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		String s = in.nextLine();
		char x = in.next().charAt(0), y = in.next().charAt(0);
		System.out.println(StringLoop.getDiff(s, x, y));
	}
}