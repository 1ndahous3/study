#include <iostream>
#include <algorithm>
#include <map>

unsigned long comb_of_pyramid(int prev_lvl, int free_elems, std::map<std::pair<int, int>, unsigned long> &cache) {
	std::pair<int, int> p(prev_lvl, free_elems);
	if (cache.find(p) != cache.end()){
		//std::cout << "use(p" << prev_lvl << ", f" << free_elems << "): " << cache[p] << std::endl;
		return cache[p];
	}
	unsigned long count = 0;
	if (free_elems == 1 || free_elems == 0)
		return 1;
	for (int i = 1; i <= std::min(prev_lvl, free_elems); i++)
		count += comb_of_pyramid(i, free_elems - i, cache);
	//std::cout << "add(p" << prev_lvl << ", f" << free_elems << "): " << count << std::endl;
	cache[p] = count;
	return count;
}

int main() {
	int a;
	std::cin >> a;
	std::map<std::pair<int, int>, unsigned long> cache;
	std::cout << comb_of_pyramid(a, a, cache);
	return 0;
}
