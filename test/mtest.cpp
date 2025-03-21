#include <iostream>
#include <chrono>
#include <set>
#include <map>
#include <vector>
#include <algorithm>

#include "tq_tree.h"
#include "tq_count_tree.h"
#include "tq_branch_tree.h"

int main() {
	srand(12444);

	tiq::tree::BranchTree<tiq::tree::BranchNode<int, int>> tp;
    std::set<int> ts;

    std::vector<int> data;
    const size_t N = 100000;
    for (size_t i=0;i<N;i++) {
		data.push_back(i);
    }
    for (size_t i=1;i<N;i++) {
		std::swap(data[i], data[rand()%i]);
    }

    auto p0 = std::chrono::system_clock::now();
    int j = 0;
    for (auto i : data) {
    	j = (j+1)%10;
        tp.insert(tp.find([i](auto n){ if (n->data() < i) return -1; return 1; }), i, j);
    }
    auto p1 = std::chrono::system_clock::now();
    for (auto i : data) {
        ts.insert(i);
    }
    auto p2 = std::chrono::system_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(p1-p0).count() << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(p2-p1).count() << std::endl;
}

void test_end_insert() {
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
}
