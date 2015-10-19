#include <iostream>

int gcd(int a, int b) {
	return ((a != 0) ? gcd(b % a, a) : b);
}

int main() {
	int num, a, b;
	std::cin >> num;
	for (int max_gcd = 0, i = 1; i <= num / 2 + num % 2; i++) {
		int curr_gcd = gcd(i, num - i);
		if (max_gcd < curr_gcd) {
			a = i;
			b = num - i;
			max_gcd = curr_gcd;
		}
	}
	std::cout << a << " " << b;
	return 0;
}
