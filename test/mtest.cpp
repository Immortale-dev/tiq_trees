#include <iostream>

#include "tq_count_tree.h"

int main() {

	using Node = Tiq::Tree::CountNode<int>;
	Tiq::Tree::CountTree<Node> t;

	t.insert(t.root(), 5);
	t.insert(t.left(t.root()), 7);
	t.insert(t.left(t.left(t.root())), 9);

	std::cout << "==INSERT_FINISHED==" << std::endl;

	std::cout << "ROOT_DATA: " << t.root()->data() << std::endl;
	std::cout << "ROOT_COUNT: " << t.root()->count() << std::endl;

	auto n = t.begin();
	while(n != t.end()) {
		std::cout << n->data() << " " << n->count() << std::endl;
		n = t.find_next(n);
	}

	return 0;
}
