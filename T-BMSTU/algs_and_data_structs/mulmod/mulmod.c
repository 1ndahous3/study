#include <stdio.h>
#include <math.h>

int main() {
	char i, isAddOverflow, isMulOverflow;
	long k = 0, a, b, m, b_temp, z, y;
	scanf("%ld %ld\n%ld\n", &a, &b, &m);
	for (i = 63; i >= 0; i--) {
		b_temp = ((long) powl(2, i) & b) == (long) powl(2, i);
		isMulOverflow = k << 1 < k;
		z = isMulOverflow ? (k % m) * (2 % m) : k * 2;
		y = a * b_temp;
		isAddOverflow = (z * y) < y && (z * y) < z;
		k = isAddOverflow ? (z % m) + (y % m) : z + y;
	}
	printf("%li", k % m);
	return 0;
}
