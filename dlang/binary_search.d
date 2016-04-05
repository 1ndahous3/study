import std.stdio, std.conv;

mixin template binSearch(T, alias compare) {
	enum search(T[] arr, T elem) {
		int mid, min = 0, max = to!int(arr.length) - 1;

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

	mixin binSearch!(int, (a, b) => a < b) ascending;
	mixin binSearch!(int, (a, b) => a > b) descending;

	ascending.search(a, 4).writeln();
	descending.search(b, 3).writeln();
	descending.search(b, 7).writeln();
}