#include <vector>
#include <utility>
#include <algorithm>

#include "tq_tree.h"

namespace TEST {
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

	class MyNode : public Tiq::Tree::Node<int> {
		public:
			using value_type = int;
	};

	using node_t = MyNode;

	using MyTree = Tiq::Tree::Tree<node_t, MyAllocator<node_t>>;

	template<typename T>
	struct bs_find {
		bs_find(T val): val_(val) {}

		int operator()(T& n) {
			if (n == val_) return 0;
			if (n > val_) return -1;
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

using namespace TEST;

DESCRIBE("Tiq::Tree::Tree", {
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

		DESCRIBE("Add 10 items with keys from 1 to 10 to the end of the tree", {
			BEFORE_EACH({
				for(int i=1;i<=10;i++){
					tree->insert(tree->end(), i);
				}
			});

			IT("should build correct structure", {
				auto b = tree->begin();
				int end_cnt = 0;
				while(!b->is_end()) {
					if (tree->left(b)->is_end()) {
						EXPECT(tree->parent(tree->left(b))).toBe(b);
						++end_cnt;
					}
					if (tree->right(b)->is_end()) {
						EXPECT(tree->parent(tree->right(b))).toBe(b);
						++end_cnt;
					}
					b = tree->find_next(b);
				}
				EXPECT(end_cnt).toBe(11);
				INFO_PRINT("End node count: ") << end_cnt;
			});

			IT("Items should be inserted", {
				EXPECT(tree->size()).toBe(10);
				for(int i=1;i<=10;i++){
					auto node = tree->find(bs_find(i));
					EXPECT(node->is_end()).toBe(false);
					EXPECT(node->data()).toBe(i);
				}
			});

			IT("should rebalance the tree", {
				auto orders = get_value_orders(tree);
				EXPECT(orders).toBeIterableEqual(std::vector<std::pair<int,int>> {{1,2}, {2,4}, {3,2}, {4,0}, {5,6}, {6,4}, {7,8}, {8,6}, {9,8}, {10,9}});
			});

			IT("should successfully add item with the same value", {
				auto node = tree->find(bs_find(8));
				auto min = tree->find_min(tree->right(node));
				auto left = min->is_end() ? min : tree->left(min);
				tree->insert(left, 8);
				auto orders = get_value_orders(tree);

				std::vector<int> vals(11);
				std::transform(orders.begin(), orders.end(), vals.begin(), [](auto v){ return v.first; });

				EXPECT(tree->size()).toBe(11);
				EXPECT(vals).toBeIterableEqual(std::vector<int>{1,2,3,4,5,6,7,8,8,9,10});
			});

			IT("should not find element in wrong subtree", {
				auto wrong_subtree = tree->right(tree->root());
				auto node = tree->find(wrong_subtree, bs_find(3));

				EXPECT(node->is_end()).toBe(true);
			});

			IT("should find element in right subtree", {
				auto right_subtree = tree->left(tree->root());
				auto node = tree->find(right_subtree, bs_find(3));

				EXPECT(node->is_end()).toBe(false);
				EXPECT(node->data()).toBe(3);
			});

			IT("should find min node", {
				auto subtree = tree->right(tree->root());
				auto node = tree->find_min(subtree);
				auto node2 = tree->find_min();

				EXPECT(node->is_end()).toBe(false);
				EXPECT(node->data()).toBe(5);
				EXPECT(node2->is_end()).toBe(false);
				EXPECT(node2->data()).toBe(1);
			});

			IT("should find max node", {
				auto subtree = tree->right(tree->root());
				auto node = tree->find_max(subtree);
				auto node2 = tree->find_max();

				EXPECT(node->is_end()).toBe(false);
				EXPECT(node->data()).toBe(10);
				EXPECT(node2->is_end()).toBe(false);
				EXPECT(node2->data()).toBe(10);
			});

			IT("shuld return the same node when find_min performed on an end node", {
				auto sub = tree->left(tree->find_min(tree->right(tree->root())));
				auto node = tree->find_min(sub);

				EXPECT(node).toBe(sub);
			});

			IT("should return the same node when find_max performed on an end node", {
				auto sub = tree->right(tree->find_max(tree->left(tree->root())));
				auto node = tree->find_min(sub);

				EXPECT(node).toBe(sub);
			});

			IT("should correctly find begin node", {
				auto node = tree->begin();

				EXPECT(node->is_end()).toBe(false);
				EXPECT(node->data()).toBe(1);
			});

			IT("should correctly find end node", {
				auto node = tree->end();
				auto prev = tree->parent(node);

				EXPECT(prev->is_end()).toBe(false);
				EXPECT(prev->data()).toBe(10);
			});

			IT("should correctly find next nodes", {
				auto n1 = tree->find_next(tree->find(bs_find(5)));
				auto n2 = tree->find_next(tree->right(tree->find(bs_find(5))));
				auto n3 = tree->find_next(tree->left(tree->find(bs_find(5))));
				auto n4 = tree->find_next(tree->find(bs_find(6)));
				auto n5 = tree->find_next(tree->find_min());
				auto n6 = tree->find_next(tree->find_max());
				auto n7 = tree->find_next(tree->right(tree->find_max()));
				auto n8 = tree->find_next(tree->left(tree->find_max()));
				auto n9 = tree->find_next(tree->right(tree->find_min()));
				auto n10 = tree->find_next(tree->left(tree->find_min()));

				EXPECT(n1->is_end()).toBe(false);
				EXPECT(n1->data()).toBe(6);
				EXPECT(n2->is_end()).toBe(false);
				EXPECT(n2->data()).toBe(6);
				EXPECT(n3->is_end()).toBe(false);
				EXPECT(n3->data()).toBe(5);
				EXPECT(n4->is_end()).toBe(false);
				EXPECT(n4->data()).toBe(7);
				EXPECT(n5->is_end()).toBe(false);
				EXPECT(n5->data()).toBe(2);
				EXPECT(n6->is_end()).toBe(true);
				EXPECT(n6).toBe(tree->end());
				EXPECT(n7->is_end()).toBe(true);
				EXPECT(n7).toBe(n6);
				EXPECT(n8->is_end()).toBe(false);
				EXPECT(n8->data()).toBe(10);
				EXPECT(n9->is_end()).toBe(false);
				EXPECT(n9->data()).toBe(2);
				EXPECT(n10->is_end()).toBe(false);
				EXPECT(n10->data()).toBe(1);
			});

			IT("should correctly find prev nodes", {
				auto n1 = tree->find_prev(tree->find(bs_find(5)));
				auto n2 = tree->find_prev(tree->right(tree->find(bs_find(5))));
				auto n3 = tree->find_prev(tree->left(tree->find(bs_find(5))));
				auto n4 = tree->find_prev(tree->find(bs_find(6)));
				auto n5 = tree->find_prev(tree->find_min());
				auto n6 = tree->find_prev(tree->find_max());
				auto n7 = tree->find_prev(tree->right(tree->find_max()));
				auto n8 = tree->find_prev(tree->left(tree->find_max()));
				auto n9 = tree->find_prev(tree->right(tree->find_min()));
				auto n10 = tree->find_prev(tree->left(tree->find_min()));

				EXPECT(n1->is_end()).toBe(false);
				EXPECT(n1->data()).toBe(4);
				EXPECT(n2->is_end()).toBe(false);
				EXPECT(n2->data()).toBe(5);
				EXPECT(n3->is_end()).toBe(false);
				EXPECT(n3->data()).toBe(4);
				EXPECT(n4->is_end()).toBe(false);
				EXPECT(n4->data()).toBe(5);
				EXPECT(n5->is_end()).toBe(true);
				EXPECT(n5).toBe(tree->left(tree->begin()));
				EXPECT(n6->is_end()).toBe(false);
				EXPECT(n6->data()).toBe(9);
				EXPECT(n7->is_end()).toBe(false);
				EXPECT(n7->data()).toBe(10);
				EXPECT(n8->is_end()).toBe(false);
				EXPECT(n8->data()).toBe(9);
				EXPECT(n9->is_end()).toBe(false);
				EXPECT(n9->data()).toBe(1);
				EXPECT(n10->is_end()).toBe(true);
				EXPECT(n10).toBe(n5);
			});

			IT("should find leaf node if the value is not found", {
				tree->erase(tree->find(bs_find(6)));

				auto n1 = tree->find(bs_find(-1));
				auto n2 = tree->find(bs_find(12));
				auto n3 = tree->find(bs_find(6));

				EXPECT(n1->is_end()).toBe(true);
				EXPECT(n1).toBe(tree->left(tree->begin()));
				EXPECT(n2->is_end()).toBe(true);
				EXPECT(n2).toBe(tree->end());
				EXPECT(n3->is_end()).toBe(true);
				EXPECT(tree->parent(n3)->data()).toBe(5);
			});

			DESCRIBE("erase left subtree", {
				BEFORE_EACH({
					tree->erase(tree->find(bs_find(1)));
					tree->erase(tree->find(bs_find(2)));
					tree->erase(tree->find(bs_find(3)));
				});

				IT("shoul correctly delete nodes", {
					auto orders = get_value_orders(tree);
					std::vector<int> vals(7);
					std::transform(orders.begin(), orders.end(), vals.begin(), [](auto val){ return val.first; });

					EXPECT(tree->size()).toBe(7);
					EXPECT(vals).toBeIterableEqual(std::vector<int>{4,5,6,7,8,9,10});
				});

				IT("should keep the structure correct", {
					auto b = tree->begin();
					int end_cnt = 0;
					while(!b->is_end()) {
						if (tree->left(b)->is_end()) {
							EXPECT(tree->parent(tree->left(b))).toBe(b);
							++end_cnt;
						}
						if (tree->right(b)->is_end()) {
							EXPECT(tree->parent(tree->right(b))).toBe(b);
							++end_cnt;
						}
						b = tree->find_next(b);
					}
					EXPECT(end_cnt).toBe(8);
					INFO_PRINT("End node count: ") << end_cnt;
				});

				IT("should rebalance the tree", {
					auto orders = get_value_orders(tree);

					EXPECT(orders).toBeIterableEqual(std::vector<std::pair<int,int>>{{4,6}, {5,4}, {6,0}, {7,8}, {8,6}, {9,8}, {10,9}});
				});
			});
		});

		DESCRIBE("Add 10 items in next order: {6,1,8,2,4,5,10,9,7,3}", {
			BEFORE_EACH({
				std::vector<int> v{6,1,8,2,4,5,10,9,7,3};
				for(auto it : v){
					tree->insert(tree->find(bs_find(it)), it);
				}
			});

			IT("should insert all items", {
				EXPECT(tree->size()).toBe(10);
				for(int i=1;i<=10;i++){
					auto node = tree->find(bs_find(i));

					EXPECT(node->is_end()).toBe(false);
					EXPECT(node->data()).toBe(i);
				}
			});

			IT("should rebalance the tree", {
				auto orders = get_value_orders(tree);

				EXPECT(orders).toBeIterableEqual(std::vector<std::pair<int,int>> { {1,2}, {2,6}, {3,4}, {4,2}, {5,4}, {6,0}, {7,8}, {8,9}, {9,6}, {10,9} });
			});

			IT("should correctly clear the tree", {
				tree->clear();

				EXPECT(tree->size()).toBe(0);
				EXPECT(tree->root()).toBe(tree->begin());
				EXPECT(tree->root()).toBe(tree->end());
				EXPECT(tree->root()->is_end()).toBe(true);
			});
		});

		DESCRIBE("Add 30 items from 1 to 30", {
			BEFORE_EACH({
				for(int i=1;i<=30;i++){
					tree->insert(tree->end(), i);
				}
			});

			IT("should successfully add items", {
				EXPECT(tree->size()).toBe(30);
				for(int i=1;i<=30;i++){
					EXPECT(tree->find(bs_find(i))->is_end()).toBe(false);
				}
			});

			DESCRIBE("Delete items from 5 to 25", {
				BEFORE_EACH({
					for(int i=5;i<25;i++){
						tree->erase(tree->find(bs_find(i)));
					}
				});

				IT("Items should be expected", {
					auto orders = get_value_orders(tree);

					EXPECT(tree->size()).toBe(10);
					EXPECT(orders).toBeIterableEqual(std::vector<std::pair<int,int>> { {1,2}, {2,25}, {3,4}, {4,2}, {25,0}, {26,28}, {27,26}, {28,25}, {29,28}, {30,29} });
				});
			});
		});
	});

	DESCRIBE("After tree destruct", {
		IT("Nodes count should be `0`", {
			EXPECT(created_node_count).toBe(0);
			INFO_PRINT("Node count: ") << created_node_count;
		});
	});
});

SCENARIO_END
