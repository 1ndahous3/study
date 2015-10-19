#include <iostream>

int main() {
	int num;
	std::cin >> num;
	for (int buff = 1, count = 1, i = 0; i < num; i++) {
		std::cout << buff << " ";
		count += 2;
		buff += count;
	}
	return 0;
}
