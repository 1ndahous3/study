unsigned long binsearch(unsigned long nel, int (*compare)(unsigned long i)) {
	unsigned long mid, min = 0, max = nel - 1, res = 0;
	while (min <= max) {
		mid = (min + max) / 2;
		switch (compare(mid)) {
		case -1:
			min = mid + 1;
			break;
		case 1:
			max = mid - 1;
			break;
		default:
			res = mid;
			break;
		}
		if (res)
			break;
	}
	return res ? res : nel;
}
