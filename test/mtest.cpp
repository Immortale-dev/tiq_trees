#include <iostream>

#include "tq_tree.h"

int main() {

	Tiq::Tree<int> t;

	t.insert(t.root(), 5);
	t.insert(t.root()->left(), 7);
	t.insert(t.root()->left()->left(), 9);

	std::cout << "==INSERT_FINISHED==" << std::endl;

	std::cout << "ROOT_DATA: " << t.root()->data() << std::endl;

	auto n = t.begin();
	while(n != t.end()) {
		std::cout << n->data() << std::endl;
		n = t.find_next(n);
	}

	return 0;
}
