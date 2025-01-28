#include <vector>
#include <functional>

#include "tq_leaf_count_tree.h"

namespace TEST_LEAF_COUNT {
	using Node = tiq::tree::LeafCountNode<int>;
	using MyTree = tiq::tree::LeafCountTree<Node>;

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
			if (n->is_leaf() && n->data() == val_) return 0;
			if (n->data() >= val_) return -1;
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
		int res = dfs(tree, tree->left(node), callback) + dfs(tree, tree->right(node), callback);
		if (node->is_leaf()) res++;
		callback(node, res);
		return res;
	}

	int calc_node_counts(MyTree* tree, Node* node, std::vector<std::pair<Node*,int>> &res) {
		if (node->is_end()) return 0;
		int r = calc_node_counts(tree, tree->left(node), res) + calc_node_counts(tree, tree->right(node), res);
		if (node->is_leaf()) r++;
		res.push_back({node, r});
		return r;
	}
}

SCENARIO_START

using namespace TEST_LEAF_COUNT;

DESCRIBE("tiq::tree::LeafCountTree", {

	DESCRIBE("init the tree", {
		MyTree* ct;
		BEFORE_EACH({
			ct = new MyTree();
		});
		AFTER_EACH({
			delete ct;
		});

		IT("should show 0 as root size", {
			EXPECT(ct->root()->count()).toBe(0);
		});

		DESCRIBE("Add 10 items with keys from 1 to 10 to the end of the tree", {
			BEFORE_EACH({
				for(int i=1;i<=10;i++){
					ct->insert(ct->end(), i);
				}
			});

			IT("should calculate counts correctly", {
				std::vector<std::pair<Node*,int>> res;
				calc_node_counts(ct, ct->root(), res);

				for (auto &p : res) {
					EXPECT(p.first->count()).toBe(p.second);
				}
			});

			IT("should assign 0 to leaves count", {
				auto nodes = get_nodes(ct);

				for (auto n : nodes) {
					if (ct->left(n)->is_end()) {
						EXPECT(ct->left(n)->count()).toBe(0);
					}
					if (ct->right(n)->is_end()) {
						EXPECT(ct->right(n)->count()).toBe(0);
					}
				}
			});

			DESCRIBE("erase left subtree", {
				BEFORE_EACH({
					ct->erase(ct->begin());
					ct->erase(ct->begin());
					ct->erase(ct->begin());
				});

				IT("should correctly update counts", {
					std::vector<std::pair<Node*,int>> res;
					calc_node_counts(ct, ct->root(), res);

					for (auto &p : res) {
						EXPECT(p.first->count()).toBe(p.second);
					}
				});
			});
		});

		DESCRIBE("Add 10 items in next order: {6,1,8,2,4,5,10,9,7,3}", {
			BEFORE_EACH({
				std::vector<int> v{6,1,8,2,4,5,10,9,7,3};
				for(auto it : v){
					auto node = ct->insert(ct->find(bs_find(it)), it);
					auto parent = ct->parent(node);
					if (!parent) continue;
					ct->insert(parent, ct->left(parent)->data());
				}
			});

			IT("should keep counts correctly", {
				std::vector<std::pair<Node*,int>> res;
				calc_node_counts(ct, ct->root(), res);

				for (auto &p : res) {
					EXPECT(p.first->count()).toBe(p.second);
				}
			});
		});

		DESCRIBE("Add 100 items in a weird order", {
			BEFORE_EACH({
				for(int i=1;i<=20;i++) {
					auto node = ct->insert(ct->find(bs_find(i)), i);
					auto parent = ct->parent(node);
					if (!parent) continue;
					ct->insert(parent, ct->left(parent)->data());
				}
				for(int i=80;i>=41;i--) {
					auto node = ct->insert(ct->find(bs_find(i)), i);
					auto parent = ct->parent(node);
					if (!parent) continue;
					ct->insert(parent, ct->left(parent)->data());
				}
				for(int i=21;i<=40;i++) {
					auto node = ct->insert(ct->find(bs_find(i)), i);
					auto parent = ct->parent(node);
					if (!parent) continue;
					ct->insert(parent, ct->left(parent)->data());
				}
				for(int i=100;i>=81;i--) {
					auto node = ct->insert(ct->find(bs_find(i)), i);
					auto parent = ct->parent(node);
					if (!parent) continue;
					ct->insert(parent, ct->left(parent)->data());
				}
			});

			IT("should calculate counts correctly", {
				dfs(ct, ct->root(), [](Node* n, int d){
					EXPECT(n->count()).toBe(d);
				});
			});

			IT("should correctly find index for all elements", {
				auto b = ct->begin();
				size_t ind = 0;
				while(!b->is_end()) {
					EXPECT(ct->find_index(b)).toBe(ind++);
					b = ct->find_next_leaf(b);
				}
			});

			IT("should find correct nth element", {
				auto b = ct->begin();
				size_t ind = 0;
				while(!b->is_end()) {
					EXPECT(ct->find_nth(ind++)).toBe(b);
					b = ct->find_next_leaf(b);
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
						EXPECT(n->count()).toBe(d);
					});
				});

				IT("should correctly find index for all elements", {
					auto b = ct->begin();
					size_t ind = 0;
					while(!b->is_end()) {
						EXPECT(ct->find_index(b)).toBe(ind++);
						b = ct->find_next_leaf(b);
					}
				});

				IT("should find correct nth element", {
					auto b = ct->begin();
					size_t ind = 0;
					while(!b->is_end()) {
						EXPECT(ct->find_nth(ind++)).toBe(b);
						b = ct->find_next_leaf(b);
					}
				});
			});
		});
	});
});

SCENARIO_END
