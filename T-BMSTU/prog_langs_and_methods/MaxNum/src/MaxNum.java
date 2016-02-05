import java.util.Arrays;
import java.util.Scanner;

class MyCompare implements Comparable<MyCompare> {
	public String line;

	public MyCompare(String line) {
		this.line = line;
	}

	public int compareTo(MyCompare num) {
		return (int) (Long.parseLong(num.line + line) - Long.parseLong(line + num.line));
	}
}

public class MaxNum {
	@SuppressWarnings("resource")
	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		int i = in.nextInt();
		MyCompare[] yolo = new MyCompare[i];

		for (int j = 0; j < i; j++)
			yolo[j] = new MyCompare(in.next());

		Arrays.sort(yolo);

		for (MyCompare x : yolo)
			System.out.print(x.line);
	}
}