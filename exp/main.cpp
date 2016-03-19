#include <iostream>
#include <sys/time.h>

#define NUM 10000
#define EXP 100000000
#define NUM_OF_CYCLES 10

using namespace std;

int function(int b, int n) {
	int r = 0;
	if (n == 0) {
		r = 1;
	} else if (n % 2 == 0) {
		r = function(b, n / 2) * function(b, n / 2);
	} else {
		r = function(b, n - 1) * b;
	}
	return r;
}

int function2(int b, int n) {
	if (!n) {
		b = n = 1;
	} else if (n % 2) {
		n = function2(b, n - 1);
	} else {
		b = n = function2(b, n / 2);
	}
	return b * n;
}

int main() {
	struct timeval time;
	double start, res1, res2;

	cout << "b = " << NUM << endl;
	cout << "n = " << EXP << endl;
	cout << "Number of cycles = " << NUM_OF_CYCLES << endl << endl;

	//////// function()
	gettimeofday(&time, NULL);
	start = time.tv_sec + (time.tv_usec / 1000000.0);

	for (int i = 0; i < NUM_OF_CYCLES; i++) {
		function(NUM, EXP);
	}

	gettimeofday(&time, NULL);
	res1 = time.tv_sec + (time.tv_usec / 1000000.0) - start;

	cout << "function(): time = " << res1 << " sec." << endl;

	//////// function2()
	gettimeofday(&time, NULL);
	start = time.tv_sec + (time.tv_usec / 1000000.0);

	for (int i = 0; i < NUM_OF_CYCLES; i++) {
		function2(NUM, EXP);
	}

	gettimeofday(&time, NULL);
	res2 = time.tv_sec + (time.tv_usec / 1000000.0) - start;

	cout << "function2(): time = " << res2 << " sec." << endl;

	////////

	cout << endl;
	cout << "Difference = " << res1 - res2 << " sec." << endl;
	cout << "Сoefficient = " << res1 / res2 << endl;

	return 0;
}