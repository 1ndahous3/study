#include <iostream>
#include "supercalc.hpp"

using namespace std;

int main()
{
	SuperCalc<int> sc(1, 3);
	sc(0, 2) = -sc(0, 0) * sc(0, 1);
	sc(0, 1) = 300 - sc(0, 0);
	sc(0, 0) = 100;
	cout << (int)sc(0, 2) << endl;
	return 0;
}