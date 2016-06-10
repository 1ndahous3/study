import std.stdio, std.conv;

template binSearch(T, alias compare) {
	long binSearch(T[] arr, T elem) {
		long mid, min = 0, max = to!long(arr.length) - 1;

		while (min <= max) {
			mid = (min + max) / 2;
			if (compare(arr[mid], elem)) {
				min = mid + 1;
			} else if (!compare(arr[mid], elem) && arr[mid] != elem) {
				max = mid - 1;
			} else {
				return mid;
			}
		}

		return -1;
	}
}

void main(string[] args) {
	int[] a = [1, 2, 3, 4, 5];
	int[] b = [5, 4, 3, 2, 1];

	alias binSearch!(int, (a, b) => a < b) ascending;
	alias binSearch!(int, (a, b) => a > b) descending;

	ascending(a, 4).writeln();
	descending(b, 3).writeln();
	descending(b, 7).writeln();
}