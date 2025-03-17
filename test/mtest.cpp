#include <iostream>
#include <chrono>
#include <set>

#include "tq_tree.h"
#include "tq_count_tree.h"
#include "tq_branch_tree.h"

int main() {
	tiq::tree::Tree<tiq::tree::Node<int>> tp;
    tiq::tree::BranchTree<tiq::tree::BranchNode<int, int>> tb;
    tiq::tree::CountTree<tiq::tree::CountNode<int>> tc;
    std::set<int> ts;

    auto p0 = std::chrono::system_clock::now();
    for (int i=0;i<100000;i++) {
        ts.insert(ts.end(), i);
    }
    auto p1 = std::chrono::system_clock::now();
    for (int i=0;i<100000;i++) {
        tp.insert(tp.end(), i);
    }
    auto p2 = std::chrono::system_clock::now();
    for (int i=0;i<100000;i++) {
        tc.insert(tc.end(), i);
    }
	auto p3 = std::chrono::system_clock::now();
	for (int i=0;i<100000;i++) {
		tb.insert(tb.end(), i, 1);
	}
    auto p4 = std::chrono::system_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(p1-p0).count() << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(p2-p1).count() << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(p3-p2).count() << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(p4-p3).count() << std::endl;
	return 0;
}
