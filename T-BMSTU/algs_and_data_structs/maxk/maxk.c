#include <stdio.h>
#include <stdlib.h>

int main() {
	int i, k, n, *p, max_add, curr_add = 0;
	scanf("%d", &n);
	p = (int*) malloc(n * sizeof(int));
	for (i = 0; i < n; i++)
		scanf("%d", &p[i]);
	scanf("%d", &k);
	for (i = 0; i < k; i++)
		curr_add += p[i];
	max_add = curr_add;
	for (; i < n; i++) {
		curr_add = curr_add - p[i - k] + p[i];
		if (curr_add > max_add)
			max_add = curr_add;
	}
	free(p);
	printf("%d", max_add);
	return 0;
}
