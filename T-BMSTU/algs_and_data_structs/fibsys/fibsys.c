#include <stdio.h>

void print_fibsys(unsigned long obj1, unsigned long obj2, unsigned long *n) {
	if (*n == 0)
		printf("0");
	if (obj2 > *n)
		return;
	print_fibsys(obj2, obj1 + obj2, n);
	if (obj2 <= *n) {
		printf("1");
		*n -= obj2;
	} else
		printf("0");
}

int main() {
	unsigned long x;
	scanf("%lu", &x);
	print_fibsys(1, 1, &x);
	return 0;
}
