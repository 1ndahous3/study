#include <iostream>

class Stack {
private:
	int *buff, size = 10, count = 0;
	void resize() {
		size *= 2;
		int *newBuff = new int[size];
		for (int i = 0; i < count; i++)
			newBuff[i] = buff[i];
		delete buff;
		buff = newBuff;
	}
public:
	Stack() {
		buff = new int[size];
	}
	~Stack() {
		delete[] buff;
	}
	int Size() {
		return this->count;
	}
	int operator[](int i) {
		return buff[i];
	}
	void push(int obj) {
		if (count == size)
			this->resize();
		buff[count++] = obj;
	}
	int pop() {
		return buff[--count];
	}
};

class Queue {
private:
	Stack *in, *out;
public:
	Queue() {
		in = new Stack();
		out = new Stack();
	}
	~Queue() {
		delete in;
		delete out;
	}
	void push(int obj) {
		in->push(obj);
	}
	int pop() {
		if (out->Size() == 0)
			while (in->Size())
				out->push(in->pop());
		return out->Size() ? out->pop() : -1;
	}
};

int main() {
	Queue a;
	int n, m, num;
	std::cin >> n;
	for (int i = 0; i < n; i++) {
		std::cin >> m >> num;
		switch (m) {
		case 3:
			a.push(num);
			break;
		case 2:
			if (a.pop() != num) {
				std::cout << "NO";
				return 0;
			}
			break;
		}
	}
	std::cout << "YES";
	return 0;
}
