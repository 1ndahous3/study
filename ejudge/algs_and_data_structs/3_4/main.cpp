#include <iostream>
#include <cmath>

int binSearch(int *array, int size, int k) {
	int pos = 0, diff = std::abs(k - array[0]);
	for (int left = 0, right = size - 1; left <= right;) {
		int mid = (left + right) / 2 + (left + right) % 2;
		int diff_buff = std::abs(array[mid] - k);
		if (diff_buff - diff <= 0 && (diff_buff - diff < 0 || mid < pos)) {
			pos = mid;
			diff = diff_buff;
		}
		if (k <= array[mid])
			right = mid - 1;
		else
			left = mid + 1;
	}
	return pos;
}

int main() {
	int n, m, *arr1, *arr2;
	std::cin >> n;
	arr1 = new int[n];
	for (int i = 0; i < n; i++)
		std::cin >> arr1[i];
	std::cin >> m;
	arr2 = new int[m];
	for (int i = 0; i < m; i++)
		std::cin >> arr2[i];
	for (int i = 0; i < m; i++)
		std::cout << binSearch(arr1, n, arr2[i]) << ' ';
	delete[] arr1;
	delete[] arr2;
	return 0;
}
