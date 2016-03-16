#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Node {
	struct Node *ptr;
	int x, y;
};

struct Edge {
	struct Node *a, *b;
	double length;
};

char node_find(struct Node *elem1, struct Node *elem2);
void node_union(struct Node *elem1, struct Node *elem2);
double algorithm(struct Edge *obj);
struct Node *node_GetTop(struct Node *elem);
int compare(const void *obj1, const void *obj2);

int main() {
	int vertexes, edges;
	double length = 0;
	struct Edge *weightEdges;
	struct Node *vrtxs;
	scanf("%d", &vertexes);
	edges = vertexes * (vertexes - 1) / 2;
	vrtxs =  (struct Node *)malloc(vertexes * sizeof(struct Node));
	weightEdges = (struct Edge *)malloc(edges * sizeof(struct Edge));

	for (int i = 0; i < vertexes; i++) {
		int a, b;
		scanf("%d %d", &a, &b);
		vrtxs[i].ptr = &vrtxs[i];
		vrtxs[i].x = a;
		vrtxs[i].y = b;
	}

	for (int i = 0, k = 0; i < vertexes; i++) {
		for (int j = i + 1; j < vertexes; j++, k++) {
			weightEdges[k].a = &vrtxs[i];
			weightEdges[k].b = &vrtxs[j];
			weightEdges[k].length = sqrt((vrtxs[i].x - vrtxs[j].x) * (vrtxs[i].x - vrtxs[j].x)	+ (vrtxs[i].y - vrtxs[j].y) * (vrtxs[i].y - vrtxs[j].y));
		}
	}

	qsort(weightEdges, edges, sizeof(struct Edge), compare);

	for (int i = 0; i < edges; i++) {
		length += algorithm(&weightEdges[i]);
	}

	printf("%.2f", length);
	free(weightEdges);
	free(vrtxs);
	return 0;
}

char node_find(struct Node *elem1, struct Node *elem2) {
	return node_GetTop(elem1) == node_GetTop(elem2) ? 1 : 0;
}

void node_union(struct Node *elem1, struct Node *elem2) {
	node_GetTop(elem1)->ptr = node_GetTop(elem2);
}

double algorithm(struct Edge *obj) {
	struct Node *foo = obj->a,
			*bar = obj->b;
	if (node_find(foo, bar))
		return 0;
	node_union(foo, bar);
	return obj->length;
}

struct Node *node_GetTop(struct Node *elem) {
	return elem->ptr == elem ? elem->ptr : (elem->ptr = node_GetTop(elem->ptr));
}

int compare(const void *obj1, const void *obj2) {
	return ((const struct Edge *)obj1)->length > ((const struct Edge *)obj2)->length ? 1 : -1;
}