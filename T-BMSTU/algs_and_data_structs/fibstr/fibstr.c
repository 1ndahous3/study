#include <stdio.h>
#include <stdlib.h>

void str_swap(char **a, char **b) {
	char *foo = *a;
	*a = *b;
	*b = foo;
}

void num_swap(int *a, int *b) {
	int foo = *a;
	*a = *b;
	*b = foo;
}

char *concat(char *a, char *b, int len_a, int len_b) {
	int i = 0, j = 0, len = len_a + len_b;
	char *foo = (char*) malloc(len * sizeof(char));
	for (; i < len_a; i++)
		foo[i] = a[i];
	for (; i < len; i++, j++)
		foo[i] = b[j];
	return foo;
}

char *get_fibstr(char **a, char **b, int *len_a, int *len_b, int n) {
	int i;
	char *foo;
	for (i = 1; i < n; i++) {
		foo = concat(*a, *b, *len_a, *len_b);
		num_swap(len_b, len_a);
		*len_b += *len_a;
		str_swap(b, a);
		str_swap(b, &foo);
		free(foo);
	}
	return *a;
}

char *fibstr(int n, int *len) {
	int len_a = 1, len_b = 1;
	char *a = (char*) malloc(len_a * sizeof(char));
	char *b = (char*) malloc(len_b * sizeof(char));
	char *c;
	a[0] = 'a';
	b[0] = 'b';
	c = get_fibstr(&a, &b, &len_a, &len_b, n);
	*len = len_a;
	free(b);
	return c;
}

int main() {
	int a, i, len;
	char *mass;
	scanf("%d", &a);
	mass = fibstr(a, &len);
	for (i = 0; i < len; i++)
		printf("%c", mass[i]);
	free(mass);
	return 0;
}
