#include <cassert>

#include "tq_leaf_tree.h"

namespace TEST_LEAF_TREE {
	int created_node_count = 0;
	void increase_node_count() {
		++created_node_count;
	}
	void decrease_node_count() {
		--created_node_count;
	}

	template <typename T> class MyAllocator {
		public:
		    typedef T value_type;
		    MyAllocator() noexcept {}
		    T* allocate(std::size_t n)
		    {
		    	increase_node_count();
		        return static_cast<T*>(::operator new(n * sizeof(T)));
		    }
		    void deallocate(T* p, [[maybe_unused]] std::size_t n) noexcept
		    {
		    	decrease_node_count();
		    	::operator delete(p);
		    }
	};

	class MyNode : public tiq::tree::LeafNode<int> {
		public:
			using value_type = int;
	};

	using node_t = MyNode;
	using MyTree = tiq::tree::LeafTree<node_t, MyAllocator<node_t>>;

	int check_tree_colors(MyTree *tree, MyNode *n, int &cnt, int current_cnt) {
		if (n->debug_get_color() == 0) {
			current_cnt++;
		}

		if (n->is_leaf()) {
			if (cnt < 0) {
				cnt = current_cnt;
			} else if(cnt != current_cnt) {
				return 1;
			}
			if (!tree->left(n)->is_end() || !tree->right(n)->is_end()) {
				return 2;
			}
		}

		int res = 0;
		if (!tree->left(n)->is_end()) {
			res = std::max(res, check_tree_colors(tree, tree->left(n), cnt, current_cnt));
		}
		if (!tree->right(n)->is_end()) {
			res = std::max(res, check_tree_colors(tree, tree->right(n), cnt, current_cnt));
		}
		return res;
	}

	int validate_tree(MyTree *tree) {
		int cnt = -1;
		return check_tree_colors(tree, tree->root(), cnt, 0);
	}

	void tree_max_height(MyTree *tree, MyNode *node, int &max_height, int current) {
		assert(node->is_end() == false);
		if (node->is_leaf()) {
			max_height = std::max(max_height, current+1);
		} else {
			tree_max_height(tree, tree->left(node), max_height, current+1);
			tree_max_height(tree, tree->right(node), max_height, current+1);
		}
	}

	void inorder_print(MyTree *tree, MyNode *node, int max_height, int current) {
		if (node->is_end()) return;
		inorder_print(tree, tree->left(node), max_height, current+1);
		for (int i=0;i<max_height-current+1;i++) std::cout << "    ";
		std::cout << "[" << (node->is_leaf() ? "o" : ".") << (node->debug_get_color() ? "r" : "b") << "]" << std::endl;
		inorder_print(tree, tree->right(node), max_height, current+1);
	}

	void print_tree(MyTree *tree) {
		int max_height = 0;
		tree_max_height(tree, tree->root(), max_height, 0);
		inorder_print(tree, tree->root(), max_height, 0);
	}

	void print_node(MyNode *node) {
		if (!node) {
			std::cout << "[" << "NULL" << "]" << std::endl;
			return;
		}
		std::cout << "[" << node->is_end() << node->is_leaf() << "]" << std::endl;
	}

	template<typename T>
	struct bs_find {
		bs_find(T val): val_(val) {}

		int operator()(const node_t* n) {
			if (n->is_leaf() && n->data() == val_) return 0;
			if (n->data() >= val_) return -1;
			return 1;
		}

		private:
			T val_;
	};

	std::vector<std::pair<int,int>> get_value_orders(MyTree* tree) {
		std::vector<std::pair<int,int>> res;
		auto start = tree->begin();
		while (!start->is_end()) {
			res.push_back({ start->data(), tree->parent(start) ? tree->parent(start)->data() : 0  });
			start = tree->find_next(start);
		}
		return res;
	}
}

SCENARIO_START

using namespace TEST_LEAF_TREE;

DESCRIBE("tiq::tree::LeafTree", {
	DESCRIBE("Initialize with int template parameter", {
		MyTree* tree;
		BEFORE_EACH({
			tree = new MyTree();
		});
		AFTER_EACH({
			delete tree;
		});

		IT("Should initialize", {
			EXPECT(tree->size()).toBe(0);
		});

		IT("should return end node", {
			EXPECT(tree->begin()).toBe(tree->end());
			EXPECT(tree->root()).toBe(tree->end());
			EXPECT(tree->find_next(tree->begin())).toBe(tree->end());
			EXPECT(tree->root()).toBe(tree->end());
			EXPECT(tree->end()->is_end()).toBe(true);
		});

		DESCRIBE("Add 30 items with keys from 1 to 30 to the end of the tree", {
			BEFORE_EACH({
				for(int i=1;i<=30;i++){
					tree->insert(tree->end(), i);
				}
			});

			IT("should correctly insert values", {
				EXPECT(tree->size()).toBe(30);
				EXPECT(validate_tree(tree)).toBe(0);
				EXPECT(created_node_count).toBe(119);
				// print_tree(tree);
			});

			IT("should iterate over leafs", {
				auto n = tree->begin();
				std::vector<int> data;
				int ind=1;
				while (!n->is_end()) {
					EXPECT(n->data()).toBe(ind++);
					n = tree->find_next_leaf(n);
				}
				EXPECT(ind == 31);
			});

			IT("should iterate over leafs backwards", {
				auto n = tree->end();
				int ind = 30;
				do {
					n = tree->find_prev_leaf(n);
					if(n->is_end()) break;
					EXPECT(n->data()).toBe(ind--);
				} while(true);
				EXPECT(ind).toBe(0);
			});
		});

		DESCRIBE("Add 100 items with keys from 1 to 100 randomly with binary search", {
			BEFORE_EACH({
				std::vector<int> vals;
				int sz = 100;
				for (int i=1;i<=sz;i++) {
					vals.push_back(i);
				}
				for (int i=1;i<sz;i++) {
					std::swap(vals[i], vals[rand()%i]);
				}
				for (auto v : vals) {
					auto node = tree->insert(tree->find(bs_find(v)), v);
					auto parent = tree->parent(node);
					if (!parent) continue;
					tree->insert(parent, tree->left(parent)->data());
				}
			});

			IT("should correctly insert values", {
				EXPECT(tree->size()).toBe(100);
				EXPECT(validate_tree(tree)).toBe(0);
				// print_tree(tree);
			});

			IT("should iterate over leafs", {
				auto n = tree->begin();
				std::vector<int> data;
				int ind=1;
				while (!n->is_end()) {
					EXPECT(n->data()).toBe(ind++);
					n = tree->find_next_leaf(n);
				}
				EXPECT(ind == 101);
			});
		});
	});
});

SCENARIO_END
