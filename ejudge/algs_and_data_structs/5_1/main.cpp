#include <iostream>

class Stack {
private:
	char *buff;
	int size = 10, count = 0;
	void resize() {
		size *= 2;
		char *newBuff = new char[size];
		for (int i = 0; i < count; i++)
			newBuff[i] = buff[i];
		delete buff;
		buff = newBuff;
	}
public:
	Stack() {
		buff = new char[size];
	}
	~Stack() {
		delete[] buff;
	}
	int Size() {
		return this->count;
	}
	char operator[](int i) {
		return buff[i];
	}
	void push(char obj) {
		if (count == size)
			this->resize();
		buff[count++] = obj;
	}
	char pop() {
		return buff[--count];
	}
	bool isEmpty() {
		return this->Size() == 0;
	}
};

char reverse(char chr) {
	switch (chr) {
	case '(':
		return ')';
	case '{':
		return '}';
	case '[':
		return ']';
	case ')':
		return '(';
	case '}':
		return '{';
	case ']':
		return '[';
	default:
		return 0;
	}
}

bool isLParen(char chr) {
	switch (chr) {
	case '{':
	case '(':
	case '[':
		return true;
	default:
		return false;
	}
}

bool validate(Stack &opn, Stack &cls, char chr) {
	if (isLParen(chr)) {
		opn.push(chr);
		return true;
	}
	if (opn.isEmpty()) {
		cls.push(chr);
		return true;
	}
	switch (chr) {
	case ']':
		return opn.pop() == '[' ? true : false;
	case ')':
		return opn.pop() == '(' ? true : false;
	case '}':
		return opn.pop() == '{' ? true : false;
	default:
		return false;
	}
}

int main() {
	Stack st_open, st_close;
	std::string str;
	std::cin >> str;
	for (int i = 0; i < str.size(); i++)
		if (!validate(st_open, st_close, str.at(i))) {
			std::cout << "IMPOSSIBLE";
			return 0;
		}
	while (!st_close.isEmpty())
		std::cout << reverse(st_close.pop());
	std::cout << str;
	while (!st_open.isEmpty())
		std::cout << reverse(st_open.pop());
	return 0;
}
