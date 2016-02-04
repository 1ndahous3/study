#include <stdio.h>
#include <stdlib.h>

char *concat(char **s, int n) {
	int i, p, j, size, str_len = 0, k = 0;
	char *d;
	for (i = 0; i < n; i++)
		str_len += strnlen(s[i], 1000);
	d = (char*) malloc(str_len * sizeof(char));
	for (i = 0; i < n; i++) {
		size = strnlen(s[i], 1000);
		for (j = k, p = 0; j < k + size; j++, p++)
			d[j] = s[i][p];
		k += size;
	}
	d = (char*) realloc(d, ++k * sizeof(char));
	d[--k] = '\0';
	return d;
}

int main() {
	int i, y, num;
	char **mass, *d;
	scanf("%d ", &num);
	mass = (char**) malloc(num * sizeof(char*));
	for (i = 0; i < num; i++) {
		mass[i] = (char*) malloc(1000 * sizeof(char));
		gets(mass[i]);
	}
	d = concat(mass, num);
	printf("%s", d);
	for (i = 0; i < num; i++)
		free(mass[i]);
	free(d);
	free(mass);
	return 0;
}
