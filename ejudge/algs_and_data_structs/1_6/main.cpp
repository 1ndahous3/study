#include <iostream>

int main() {
	int size, *arr;
	std::cin >> size;
	arr = new int[size];
	for (int i = 0; i < size; i++)
		std::cin >> arr[i];
	for (int i = 0; i < size / 2; i++)
		std::swap(arr[i], arr[size - i - 1]);
	for (int i = 0; i < size; i++)
		std::cout << arr[i] << " ";
	delete[] arr;
}
