import java.util.ArrayList;
import java.util.Scanner;

public class MaxNum {
	@SuppressWarnings("resource")
	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		int nel = in.nextInt();
		ArrayList<Long> nums = new ArrayList<>();

		for (int i = 0; i < nel; i++)
			nums.add(in.nextLong());

		nums
				.stream()
				.sorted((a, b) -> Long.compare(Long.parseLong(b.toString() + a.toString()), Long.parseLong((a.toString() + b.toString()))))
				.forEach(System.out::print);
	}
}