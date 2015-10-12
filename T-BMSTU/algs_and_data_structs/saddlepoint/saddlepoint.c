#include <stdio.h>
#include <stdlib.h>

#define MAX_OF_VALUE_TYPE ((1 << 31) - 1) // int

int main() {
	int **c, **x_arr, **y_arr, a, b, i, j, x, y;
	int buff_min = 0, buff_max = MAX_OF_VALUE_TYPE;
	char isPointExist = 0;
	scanf("%d%d", &a, &b);
	x_arr = (int**) malloc(a * sizeof(int*));
	y_arr = (int**) malloc(b * sizeof(int*));
	c = (int**) malloc(a * sizeof(int*));
	for (i = 0; i < a; i++)
		c[i] = (int*) malloc(b * sizeof(int));
	for (i = 0; i < a; i++)
		x_arr[i] = (int*) malloc(2 * sizeof(int));
	for (i = 0; i < b; i++)
		y_arr[i] = (int*) malloc(2 * sizeof(int));
	for (i = 0; i < a; i++)
		for (j = 0; j < b; j++)
			scanf("%d", &c[i][j]);
	for (i = 0; i < a; i++) {
		for (j = 0; j < b; j++)
			if (c[i][j] >= buff_min) {
				buff_min = c[i][j];
				x_arr[i][0] = i;
				x_arr[i][1] = j;
			}
		buff_min = 0;
	}
	for (i = 0; i < b; i++) {
		for (j = 0; j < a; j++)
			if (c[j][i] <= buff_max) {
				buff_max = c[j][i];
				y_arr[i][0] = j;
				y_arr[i][1] = i;
			}
		buff_max = MAX_OF_VALUE_TYPE;
	}
	for (i = 0; i < a; i++)
		for (j = 0; j < b; j++)
			if ((x_arr[i][0] == y_arr[j][0])
					&& (x_arr[i][1] == y_arr[j][1])) {
				isPointExist++;
				x = i;
				y = j;
			}
	printf(isPointExist ? "%d %d", x, y : "none");
	for (i = 0; i < b; i++)
		free(y_arr[i]);
	for (i = 0; i < a; i++) {
		free(x_arr[i]);
		free(c[i]);
	}
	free(c);
	free(x_arr);
	free(y_arr);
	return 0;
}
