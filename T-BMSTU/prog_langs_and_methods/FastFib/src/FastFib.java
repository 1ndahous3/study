import java.math.BigInteger;
import java.util.Scanner;

public class FastFib {
	private static BigInteger[][] multiplication(BigInteger[][] a, BigInteger[][] b) {
		BigInteger[][] M = {
				{ (a[0][0].multiply(b[0][0])).add(a[0][1].multiply(b[1][0])), (a[0][0].multiply(b[0][1])).add(a[0][1].multiply(b[1][1])) },
				{ (a[1][0].multiply(b[0][0])).add(a[1][1].multiply(b[1][0])), (a[1][0].multiply(b[0][1])).add(a[1][1].multiply(b[1][1])) } };
		return M;
	}

	private static BigInteger[][] exponentiation(int n) {
		BigInteger[][] E = {
				{ BigInteger.ONE, BigInteger.ONE },
				{ BigInteger.ONE, BigInteger.ZERO } };

		return n == 1 ? E
				: n % 2 == 1 ?
				multiplication(E, exponentiation(n - 1))
				: multiplication(exponentiation(n / 2), exponentiation(n / 2));
	}

	private static BigInteger GetFib(int n) {
		// step 1: get square matrix
		BigInteger[][] M = exponentiation(n);

		// step 2: get line matrix
		BigInteger[] L = { M[0][0].add(M[1][0]), M[0][1].add(M[1][1]) };
		
		return L[0].subtract(L[1]);
	}

	@SuppressWarnings("resource")
	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		System.out.println(FastFib.GetFib(in.nextInt()));
	}
}