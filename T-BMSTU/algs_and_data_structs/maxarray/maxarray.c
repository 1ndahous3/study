int maxarray(void* base, unsigned long nel, unsigned long width,
		int (*compare)(void *a, void *b)) {
	unsigned long i, k = 1;
	for (i = 0; i < (nel - 1); i++)
		if (compare((void*) ((unsigned long) base + i * width),
				(void*) ((unsigned long) base + k * width)) > 0)
			k = i;
	return k;
}
