#include "qtest.hpp"

#include <vector>
#include <functional>

#include "tq_count_tree.h"

namespace TEST_COUNT {
	using Node = tiq::tree::CountNode<int>;
	using MyTree = tiq::tree::CountTree<Node>;

	std::vector<Node*> get_nodes(MyTree* tree) {
		std::vector<Node*> res;
		auto start = tree->begin();
		while (!start->is_end()) {
			res.push_back(start);
			start = tree->find_next(start);
		}
		return res;
	}

	template<typename T>
	struct bs_find {
		bs_find(T val): val_(val) {}

		int operator()(const Node* n) {
			if (n->data() == val_) return 0;
			if (n->data() > val_) return -1;
			return 1;
		}

		private:
			T val_;
	};

	int dfs(MyTree* tree, Node* node, std::function<void(Node*,int)> callback) {
		if (node->is_end()) {
			callback(node, 0);
			return 0;
		}
		int res = dfs(tree, tree->left(node), callback) + dfs(tree, tree->right(node), callback) + 1;
		callback(node, res);
		return res;
	}
}

SCENARIO_START

using namespace TEST_COUNT;

DESCRIBE("tiq::tree::CountTree", {

	DESCRIBE("init the tree", {
		MyTree* ct;
		BEFORE_EACH({
			ct = new MyTree();
		});
		AFTER_EACH({
			delete ct;
		});

		IT("should show 0 as root size", {
			EXPECT(ct->root()->count()).toBe(0ull);
		});

		DESCRIBE("Add 10 items with keys from 1 to 10 to the end of the tree", {
			BEFORE_EACH({
				for(int i=1;i<=10;i++){
					ct->insert(ct->end(), i);
				}
			});

			IT("should calculate counts correctly", {
				auto nodes = get_nodes(ct);

				std::vector<size_t> counts(10);
				std::transform(nodes.begin(), nodes.end(), counts.begin(), [](auto n){ return n->count(); });

				EXPECT(counts).toBeIterableEqual(std::vector<size_t>{1, 3, 1, 10, 1, 6, 1, 4, 2, 1});
			});

			IT("should assign 0 to leaves count", {
				auto nodes = get_nodes(ct);

				for (auto n : nodes) {
					if (ct->left(n)->is_end()) {
						EXPECT(ct->left(n)->count()).toBe(0ull);
					}
					if (ct->right(n)->is_end()) {
						EXPECT(ct->right(n)->count()).toBe(0ull);
					}
				}
			});

			DESCRIBE("erase left subtree", {
				BEFORE_EACH({
					ct->erase(ct->find(bs_find(1)));
					ct->erase(ct->find(bs_find(2)));
					ct->erase(ct->find(bs_find(3)));
				});

				IT("should correctly update counts", {
					auto nodes = get_nodes(ct);

					std::vector<int> counts(7);
					std::transform(nodes.begin(), nodes.end(), counts.begin(), [](auto n){ return n->count(); });

					EXPECT(counts).toBeIterableEqual(std::vector<int>{2, 1, 7, 1, 4, 2, 1});
				});
			});
		});

		DESCRIBE("Add 10 items in next order: {6,1,8,2,4,5,10,9,7,3}", {
			BEFORE_EACH({
				std::vector<int> v{6,1,8,2,4,5,10,9,7,3};
				for(auto it : v){
					ct->insert(ct->find(bs_find(it)), it);
				}
			});

			IT("should keep counts correctly", {
				auto nodes = get_nodes(ct);

				std::vector<int> counts(10);
				std::transform(nodes.begin(), nodes.end(), counts.begin(), [](auto n){ return n->count(); });

				EXPECT(counts).toBeIterableEqual(std::vector<int>{1, 5, 1, 3, 1, 10, 1, 2, 4, 1});
			});
		});

		DESCRIBE("Add 100 items in a weird order", {
			BEFORE_EACH({
				for(int i=1;i<=20;i++) {
					ct->insert(ct->find(bs_find(i)), i);
				}
				for(int i=80;i>=41;i--) {
					ct->insert(ct->find(bs_find(i)), i);
				}
				for(int i=21;i<=40;i++) {
					ct->insert(ct->find(bs_find(i)), i);
				}
				for(int i=100;i>=81;i--) {
					ct->insert(ct->find(bs_find(i)), i);
				}
			});

			IT("should calculate counts correctly", {
				dfs(ct, ct->root(), [](Node* n, int d){
					EXPECT(n->count()).toBe((size_t)d);
				});
			});

			IT("should correctly find index for all elements", {
				auto b = ct->begin();
				size_t ind = 0;
				while(!b->is_end()) {
					EXPECT(ct->find_index(b)).toBe(ind++);
					b = ct->find_next(b);
				}
			});

			IT("should find correct nth element", {
				auto b = ct->begin();
				size_t ind = 0;
				while(!b->is_end()) {
					EXPECT(ct->find_nth(ind++)).toBe(b);
					b = ct->find_next(b);
				}
			});

			DESCRIBE("Remove 40 items in a weird order", {
				BEFORE_EACH({
					for (int i=21;i<=40;i++) {
						ct->erase(ct->find(bs_find(i)));
					}
					for (int i=80;i>=71;i--) {
						ct->erase(ct->find(bs_find(i)));
					}
					for (int i=61;i<=70;i++) {
						ct->erase(ct->find(bs_find(i)));
					}
				});

				IT("should update counts correctly", {
					dfs(ct, ct->root(), [](Node* n, int d){
						EXPECT(n->count()).toBe((size_t)d);
					});
				});

				IT("should correctly find index for all elements", {
					auto b = ct->begin();
					size_t ind = 0;
					while(!b->is_end()) {
						EXPECT(ct->find_index(b)).toBe(ind++);
						b = ct->find_next(b);
					}
				});

				IT("should find correct nth element", {
					auto b = ct->begin();
					size_t ind = 0;
					while(!b->is_end()) {
						EXPECT(ct->find_nth(ind++)).toBe(b);
						b = ct->find_next(b);
					}
				});
			});
		});
	});
});

SCENARIO_END
