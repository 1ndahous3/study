#include <stdio.h>

int main() {
	unsigned int i, num, a = 0, b = 0, c;
	scanf("%d", &c);
	for (i = 0; i < c; i++) {
		scanf("%d", &num);
		a += 1 << num;
	}
	scanf("%d", &c);
	for (i = 0; i < c; i++) {
		scanf("%d", &num);
		b += 1 << num;
	}
	for (i = 0; i < 32; i++)
		if (a & b & (1 << i))
			printf("%d ", i);
	return 0;
}
