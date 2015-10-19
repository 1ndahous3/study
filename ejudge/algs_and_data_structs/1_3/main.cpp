#include <iostream>

int gcd(int a, int b) {
	return ((a != 0) ? gcd(b % a, a) : b);
}

int lcm(int a, int b) {
	return a / gcd(a, b) * b;
}

class rational {
private:
	int num = 0, denom = 1;
	void simplify() {
		int curr_denom = gcd(this->num, this->denom);
		if (curr_denom != 0) {
			this->num /= curr_denom;
			this->denom /= curr_denom;
		}
	}
public:
	rational(int a, int b) {
		this->num = a;
		this->denom = b;
	}
	friend rational operator+ (rational obj1, rational obj2) {
		int curr_denom = lcm(obj1.denom, obj2.denom);
		obj1.num *= curr_denom / obj1.denom;
		obj2.num *= curr_denom / obj2.denom;
		rational res(obj1.num + obj2.num, curr_denom);
		res.simplify();
		return res;
	}
	void print() {
		std::cout << this->num << " " << this->denom;
	}
};

int main() {
	int a, b, c, d;
	std::cin >> a >> b >> c >> d;
	rational num1(a, b), num2(c, d);
	rational res = num1 + num2;
	res.print();
	return 0;
}
