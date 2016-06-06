#include <functional>
#include <experimental/optional>
#include <vector>

using namespace std;
using namespace experimental;

template<class T>
class Node {
private:
	optional<T> value;
	function<T()> func;

public:
	Node() : func([=]() { return value.value(); }) {};

	Node(T _value) : value(_value), func([value = value]() { return value.value() ; }) {};

	Node(function<T()> &&_func) : func(_func) {};

	Node(const Node<T> &rhs) : func(rhs.func){}

	Node<T> &operator=(const Node<T> &rhs) {
		func = [&rhs]() { return rhs.func(); };
		return *this;
	}

	Node<T> &operator=(Node<T> &&rhs) {
		if (rhs.value) {
			value = rhs.value;
			func = [&value = value]() { return value.value(); };
		} else {
			func = rhs.func;
			value = optional<T>();
		}
		return *this;
	}

	Node<T> &operator+=(const Node<T> &rhs) {
		func = [&rhs, lhs_f = func]() { return lhs_f() + rhs.func(); };
		return *this;
	}

	Node<T> &operator+=(Node<T> &&rhs) {
		func = [rhs_f = rhs.func, lhs_f = func]() { return lhs_f() + rhs_f(); };
		return *this;
	}

	Node<T> &operator-=(const Node<T> &rhs) {
		func = [&rhs, lhs_f = func]() { return lhs_f() - rhs.func(); };
		return *this;
	}

	Node<T> &operator-=(Node<T> &&rhs) {
		func = [rhs_f = rhs.func, lhs_f = func]() { return lhs_f() - rhs_f(); };
		return *this;
	}

	Node<T> &operator*=(const Node<T> &rhs) {
		func = [&rhs, lhs_f = func]() { return lhs_f() * rhs.func(); };
		return *this;
	}

	Node<T> &operator*=(Node<T> &&rhs) {
		func = [rhs_f = rhs.func, lhs_f = func]() { return lhs_f() * rhs_f(); };
		return *this;
	}

	Node<T> &operator/=(const Node<T> &rhs) {
		func = [&rhs, lhs_f = func]() { return lhs_f() / rhs.func(); };
		return *this;
	}

	Node<T> &operator/=(Node<T> &&rhs) {
		func = [rhs_f = rhs.func, lhs_f = func]() { return lhs_f() * rhs_f(); };
		return *this;
	}

	Node<T> operator-() {
		return Node([&]() { return -func(); });
	}

	friend Node<T> operator+(const Node<T> &lhs, const Node<T> &rhs) {
		return Node([&lhs, &rhs]() { return lhs.func() + rhs.func(); });
	}

	friend Node<T> operator+(const Node<T> &lhs, Node<T> &&rhs) {
		return Node([&lhs, rhs_f = rhs.func]() { return lhs.func() + rhs_f(); });
	}

	friend Node<T> operator+(Node<T> &&lhs, const Node<T> &rhs) {
		return Node([lhs_f = lhs.func, &rhs]() { return lhs_f() + rhs.func(); });
	}

	friend Node<T> operator+(Node<T> &&lhs, Node<T> &&rhs) {
		return Node([lhs_f = lhs.func, rhs_f = rhs.func]() { return lhs_f() + rhs_f(); });
	}

	friend Node<T> operator-(const Node<T> &lhs, const Node<T> &rhs) {
		return Node([&lhs, &rhs]() { return lhs.func() - rhs.func(); });
	}

	friend Node<T> operator-(const Node<T> &lhs, Node<T> &&rhs) {
		return Node([&lhs, rhs_f = rhs.func]() { return lhs.func() - rhs_f(); });
	}

	friend Node<T> operator-(Node<T> &&lhs, const Node<T> &rhs) {
		return Node([lhs_f = lhs.func, &rhs]() { return lhs_f() - rhs.func(); });
	}

	friend Node<T> operator-(Node<T> &&lhs, Node<T> &&rhs) {
		return Node([lhs_f = lhs.func, rhs_f = rhs.func]() { return lhs_f() - rhs_f(); });
	}

	friend Node<T> operator*(const Node<T> &lhs, const Node<T> &rhs) {
		return Node([&lhs, &rhs]() { return lhs.func() * rhs.func(); });
	}

	friend Node<T> operator*(const Node<T> &lhs, Node<T> &&rhs) {
		return Node([&lhs, rhs_f = rhs.func]() { return lhs.func() * rhs_f(); });
	}

	friend Node<T> operator*(Node<T> &&lhs, const Node<T> &rhs) {
		return Node([lhs_f = lhs.func, &rhs]() { return lhs_f() * rhs.func(); });
	}

	friend Node<T> operator*(Node<T> &&lhs, Node<T> &&rhs) {
		return Node([lhs_f = lhs.func, rhs_f = rhs.func]() { return lhs_f() * rhs_f(); });
	}

	friend Node<T> operator/(const Node<T> &lhs, const Node<T> &rhs) {
		return Node([&lhs, &rhs]() { return lhs.func() / rhs.func(); });
	}

	friend Node<T> operator/(const Node<T> &lhs, Node<T> &&rhs) {
		return Node([&lhs, rhs_f = rhs.func]() { return lhs.func() / rhs_f(); });
	}

	friend Node<T> operator/(Node<T> &&lhs, const Node<T> &rhs) {
		return Node([lhs_f = lhs.func, &rhs]() { return lhs_f() / rhs.func(); });
	}

	friend Node<T> operator/(Node<T> &&lhs, Node<T> &&rhs) {
		return Node([lhs_f = lhs.func, rhs_f = rhs.func]() { return lhs_f() / rhs_f(); });
	}

	explicit operator T() const {
		return func();
	}
};

template<class T>
class SuperCalc {
private:
	vector<vector<Node<T>>> vec;

public:
	SuperCalc(int m, int n) : vec(m) {
		for (int i = 0; i < m; i++) {
			vec.emplace_back(n);
			for (int j = 0; j < n; j++) {
				vec[i].emplace_back();
			}
		}
	}

	Node<T> &operator()(int i, int j) {
		return vec[i][j];
	}
};