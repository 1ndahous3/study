#include <iostream>
#include <limits>

int main() {
	int nel, max = std::numeric_limits<int>::min(), *arr1, *arr2;
	int i0 = 0, j0 = 0, i_last = 0;
	std::cin >> nel;
	arr1 = new int[nel];
	arr2 = new int[nel];
	for (int i = 0; i < nel; i++)
		std::cin >> arr1[i];
	for (int i = 0; i < nel; i++)
		std::cin >> arr2[i];
	for (int k = 1; k < nel; k++) {
		if (arr1[k] > arr1[i_last])
			i_last = k;
		if (arr1[i_last] + arr2[k] > max) {
			i0 = i_last;
			j0 = k;
			max = arr1[i_last] + arr2[j0];
		}
	}
	std::cout << i0 << ' ' << j0;
	delete[] arr1;
	delete[] arr2;
	return 0;
}
