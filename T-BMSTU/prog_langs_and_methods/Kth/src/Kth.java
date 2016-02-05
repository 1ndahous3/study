import java.util.Scanner;
import java.lang.Math;

class calc {
	static long number(long count) {
		long i = 1, j = 10, m = 9, num, key;
		count++;

		while (count - i * m > 0) {
			count -= i++ * m;
			j *= 10;
			m *= 10;
		}
		j /= 10;
		key = count % i;
		num = j + count / i;

		return (key == 0 ? --num : num / (long) Math.pow(10, i - key)) % 10;
	}
}

public class Kth {
	@SuppressWarnings("resource")
	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		System.out.println(calc.number(in.nextLong()));
	}
}