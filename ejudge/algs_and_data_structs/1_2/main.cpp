#include <iostream>

int main() {
	int num;
	std::cin >> num;
	if (num == 1)
		std::cout << num;
	for (int i = 2; i * i <= num; i++)
		for (; !(num % i); num /= i)
			std::cout << i << " ";
	if (num != 1)
		std::cout << num;
	return 0;
}
