#include <iostream>

int main() {
	int size, count_of_two = 0, count_of_five = 0;
	std::cin >> size;
	for (int i = 0; i < size; i++) {
		int num = i + 1;
		for (; !(num % 2); count_of_two++, num /= 2);
		for (; !(num % 5); count_of_five++, num /= 5);
	}
	std::cout << std::min(count_of_two, count_of_five);
	return 0;
}
