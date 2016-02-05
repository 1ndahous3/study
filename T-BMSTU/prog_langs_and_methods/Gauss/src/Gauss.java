import java.util.Scanner;

class MultiNum {
	public int num = 0;
	public int denom = 1;

	MultiNum(int a, int b) {
		num = a;
		denom = b;
	}

	MultiNum(MultiNum a) {
		this(a.num, a.denom);
	}

	MultiNum(int a) {
		num = a;
	}

	void div(MultiNum a) {
		num *= a.denom;
		denom *= a.num;
		this.simplify();
	}

	void mult(MultiNum a) {
		num *= a.num;
		denom *= a.denom;
		this.simplify();
	}

	void substr(MultiNum a) {
		if (a.num == 0)
			return;
		MultiNum tmp = new MultiNum(a);
		int curr_denom = lcm(a.denom, this.denom);
		if (num != 0) {
			tmp.num *= curr_denom / tmp.denom;
			num *= curr_denom / denom;
			num -= tmp.num;
			denom = curr_denom;
		} else {
			num = -a.num;
			denom = a.denom;
		}
		this.simplify();
	}

	private void simplify() {
		int curr_denom = gcd(this.num, this.denom);
		if (this.denom == 0 || curr_denom == 0)
			this.setZero();
		else if (curr_denom != 0) {
			num /= curr_denom;
			denom /= curr_denom;
			if (denom < 0) {
				num = -num;
				denom = -denom;
			}
		}
	}

	private void setZero() {
		this.num = 0;
		this.denom = 1;
	}

	boolean isZero() {
		return this.num == 0;
	}

	void swap(MultiNum obj) {
		int a = this.num, b = this.denom;
		this.num = obj.num;
		this.denom = obj.denom;
		obj.num = a;
		obj.denom = b;
	}

	private static int gcd(int a, int b) {
		return (a != 0 ? gcd(b % a, a) : b);
	}

	private static int lcm(int a, int b) {
		return a / gcd(a, b) * b;
	}
}

class GaussMatrix {
	private int n;
	private MultiNum[][] mtrx;

	GaussMatrix(int nel, int[][] orig) {
		n = nel;
		mtrx = new MultiNum[n][n + 1];
		for (int i = 0; i < n; i++) {
			mtrx[i] = new MultiNum[n + 1];
			for (int j = 0; j < n + 1; j++)
				mtrx[i][j] = new MultiNum(orig[i][j]);
		}
	}

	void Gauss_func() {
		// direct Gauss way
		for (int i = 0; i < n - 1; i++) {
			if (mtrx[i][i].isZero())
				for (int j = i + 1; j < n; j++) {
					if (!mtrx[j][i].isZero()) {
						for (int k = 0; k <= n; k++)
							mtrx[j][k].swap(mtrx[i][k]);
						break;
					}
				}
			for (int j = i + 1; j < n; j++) {
				MultiNum curr_elem_div = new MultiNum(mtrx[i][i]);
				MultiNum curr_elem_mult = new MultiNum(mtrx[j][i]);
				MultiNum[] curr_line = new MultiNum[n + 1];
				for (int k = 0; k <= n; k++) {
					curr_line[k] = new MultiNum(mtrx[i][k]);
					curr_line[k].div(curr_elem_div);
					curr_line[k].mult(curr_elem_mult);
					mtrx[j][k].substr(curr_line[k]);
				}
			}
		}
		// reverse Gauss way
		for (int i = n - 1; i > 0; i--) {
			for (int j = i - 1; j >= 0; j--) {
				MultiNum curr_elem_div = new MultiNum(mtrx[i][i]);
				MultiNum curr_elem_mult = new MultiNum(mtrx[j][i]);
				MultiNum[] curr_line = new MultiNum[n + 1];
				for (int k = 0; k <= n; k++) {
					curr_line[k] = new MultiNum(mtrx[i][k]);
					curr_line[k].div(curr_elem_div);
					curr_line[k].mult(curr_elem_mult);
					mtrx[j][k].substr(curr_line[k]);
				}
			}
		}
	}

	void print_sollution() {
		for (int i = 0; i < n; i++)
			if (mtrx[i][i].isZero()) {
				System.out.println("No solution");
				return;
			}
		for (int i = 0; i < n; i++)
			mtrx[i][n].div(mtrx[i][i]);
		for (int i = 0; i < n; i++)
			System.out.println(mtrx[i][n].num + "/" + mtrx[i][n].denom);
	}
	/*
	 * void print_mtrx() { for (int i = 0; i < n; i++) { for (int j = 0; j < n;
	 * j++) System.out.print(mtrx[i][j].num + "/" + mtrx[i][j].denom + "\t");
	 * System.out.println("| " + mtrx[i][n].num + "/" + mtrx[i][n].denom); }
	 * System.out.println(); }
	 */
}

public class Gauss {
	public static void main(String args[]) {
		@SuppressWarnings("resource")
		Scanner in = new Scanner(System.in);
		int n = in.nextInt();
		int[][] mtrx = new int[n][n + 1];
		for (int i = 0; i < n; i++) {
			mtrx[i] = new int[n + 1];
			for (int j = 0; j < n + 1; j++) {
				mtrx[i][j] = in.nextInt();
			}
		}
		GaussMatrix res_mtrx = new GaussMatrix(n, mtrx);
		res_mtrx.Gauss_func();
		// res_mtrx.print_mtrx();
		res_mtrx.print_sollution();
	}
}