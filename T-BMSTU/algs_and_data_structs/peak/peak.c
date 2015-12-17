unsigned long peak(unsigned long nel,
		int (*less)(unsigned long i, unsigned long j)) {
	unsigned long mid, left = 0, right = nel - 1;
	if (right == 1)
		return less(0, 1);
	if (less(right - 1, right))
		return right;
	for (;;) {
		mid = ((left % 2 + right % 2) / 2 + left / 2 + right / 2);
		if (!less(mid, mid - 1) && !less(mid, mid + 1))
			return mid;
		if (less(mid - 1, mid) || less(mid, mid + 1))
			left = mid - 1;
		else
			right = mid + 1;
	}
}
