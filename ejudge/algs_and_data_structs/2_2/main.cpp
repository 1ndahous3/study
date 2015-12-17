#include <iostream>
#include <vector>
#include <cmath>

struct point {
	int x=0, y=0;
};

float getSquare(int x1, int y1, int x2, int y2, int x3, int y3) {
	return std::abs(0.5 * ((x1 - x3) * (y2 - y3) - (x2 - x3) * (y1 - y3)));
}

int main() {
	int nel;
	float square = 0;
	std::vector<point> arr;
	std::cin >> nel;
	for (int i = 0; i < nel; i++) {
		point elem;
		std::cin >> elem.x >> elem.y;
		arr.push_back(elem);
	}
	for (int i = 1; i < nel - 1; i++)
		square += getSquare(arr[0].x, arr[0].y, arr[i].x, arr[i].y, arr[i+1].x, arr[i+1].y);
	std::cout << square;
	return 0;
}
