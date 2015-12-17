#include "undo_container.hpp"
#include <string>
#include <iostream>


int main() {
	undo_container<std::string> foo;

	std::string a = "foo_asdasd_bar";
	std::string b = "foo__asdbar";

	foo.push_diffs(a, b);

	for (auto i : foo)
		std::cout << i.type << ' ' << i.obj;

	return 0;
}
