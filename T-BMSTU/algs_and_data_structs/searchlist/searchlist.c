#include "elem.h"

struct Elem *searchlist(struct Elem *list, int k) {
	for (; list; list = list->tail) {
		if ((list->tag == 2) && (searchlist(list->value.list, k)))
			return searchlist(list->value.list, k);
		if (list->tag || (list->value.i != k))
			continue;
		return list;
	}
	return 0;
}
