#include <iostream>
#include <chrono>
#include <set>
#include <map>
#include <vector>
#include <algorithm>

#include "tq_tree.h"
#include "tq_count_tree.h"
#include "tq_branch_tree.h"

void test_end_insert() {
	tiq::tree::Tree<tiq::tree::Node<int>> tp;
    tiq::tree::BranchTree<tiq::tree::BranchNode<int, int>> tb;
    tiq::tree::CountTree<tiq::tree::CountNode<int>> tc;
    std::set<int> ts;

    const int N = 100000;

    auto p0 = std::chrono::system_clock::now();
    for (int i=0;i<N;i++) {
        ts.insert(ts.end(), i);
    }
    auto p1 = std::chrono::system_clock::now();
    for (int i=0;i<N;i++) {
        tp.insert(tp.end(), i);
    }
    auto p2 = std::chrono::system_clock::now();
    for (int i=0;i<N;i++) {
        tc.insert(tc.end(), i);
    }
	auto p3 = std::chrono::system_clock::now();

	std::cout << "TEST END INSERT" << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(p1-p0).count() << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(p2-p1).count() << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(p3-p2).count() << std::endl;
    std::cout << std::endl;
}

void test_random_insert() {
	tiq::tree::Tree<tiq::tree::Node<int>> tp;
    tiq::tree::BranchTree<tiq::tree::BranchNode<int, int>> tb;
    tiq::tree::CountTree<tiq::tree::CountNode<int>> tc;
    std::set<int> ts;

    const int N = 100000;
    std::vector<int> data;
    for (int i=0;i<N;i++) {
		data.push_back(i);
    }
    for (int i=1;i<N;i++) {
		std::swap(data[i], data[rand()%i]);
    }

    auto p0 = std::chrono::system_clock::now();
    for (int i=0;i<N;i++) {
        ts.insert(i);
    }
    auto p1 = std::chrono::system_clock::now();
    for (int i=0;i<N;i++) {
    	auto &d = data[i];
        tp.insert(tp.find([&d](auto a){ return d > a->data() ? -1 : 1; }), i);
    }
    auto p2 = std::chrono::system_clock::now();
    for (int i=0;i<N;i++) {
    	auto &d = data[i];
        tc.insert(tc.find([&d](auto a){ return d > a->data() ? -1 : 1; }), i);
    }
	auto p3 = std::chrono::system_clock::now();

	std::cout << "TEST RANDOM INSERT" << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(p1-p0).count() << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(p2-p1).count() << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(p3-p2).count() << std::endl;
    std::cout << std::endl;
}

int main() {
	srand(12444);

	test_end_insert();
	test_random_insert();
}
