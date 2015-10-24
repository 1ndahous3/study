#include <iostream>
#include <cmath>

int binSearch(int *array, int min, int max, int k) {
	int pos = 0, diff = std::abs(k - array[0]);
	for (int left = min, right = max; left <= right;) {
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

int gallopSearch(int *array, int size, int n, int k) {
	int i = 0;
	int j = i + 1;
	if (k > array[n]) {
		for (; n + j < size; i = j, j *= 2)
			if (array[n + j] >= k)
				return binSearch(array, n + i, n + j, k);
		return binSearch(array, n + i, size - 1, k);
	} else {
		for (; n - j >= 0; i = j, j *= 2)
			if (array[n - j] <= k)
				return binSearch(array, n - j, n - i, k);
		return binSearch(array, 0, n - i, k);
	}
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
	for (int i = 0, k = 0; i < m; i++) {
		k = gallopSearch(arr1, n, k, arr2[i]);
		std::cout << k << ' ';
	}
	delete[] arr1;
	delete[] arr2;
	return 0;
}
