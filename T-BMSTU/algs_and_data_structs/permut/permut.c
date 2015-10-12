#include <stdio.h>

int main() {
	int i, j, a[8], b[8];
	for (i = 0; i < 8; i++)
		scanf("%d", &a[i]);
	for (i = 0; i < 8; i++)
		scanf("%d", &b[i]);
	for (i = 0; i < 8;) {
		for (j = 0; j < 8; j++)
			if (a[i] == b[j]) {
				a[i] = b[j] = 0;
				goto end;
			}
		printf("no");
		return 0;
		end: i++;
	}
	printf("yes");
	return 0;
}
