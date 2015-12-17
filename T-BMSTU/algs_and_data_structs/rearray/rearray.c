void revarray(void *base, unsigned long nel, unsigned long width) {
	char *arr = (char*) base;
	int i, j;
	char elem;
	for (i = 0; i < nel / 2; i++)
		for (j = 0; j < width; j++) {
			elem = arr[i * width + j];
			arr[i * width + j] = arr[width * (nel - i - 1) + j];
			arr[width * (nel - 1) - i * width + j] = elem;
		}
}
