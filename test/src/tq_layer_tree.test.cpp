#include <vector>

#include "tq_layer_tree.h"

namespace TEST_LAYER {
	using Node = Tiq::Tree::LayerNode<int, int>;
	using MyTree = Tiq::Tree::LayerTree<Node>;
	using CollectionTree = Tiq::Tree::LayersCollection<int>;
	using ValueTree = Tiq::Tree::ValuesCollection<int,int>;

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

		int operator()(T& n) {
			if (n == val_) return 0;
			if (n > val_) return -1;
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

	void print_tree(MyTree* tree) {
		dfs(tree, tree->root(), [tree](Node* n, [[maybe_unused]]int a){
			if (n->is_end()) return 0;
			std::cout << n->data() << " - " << (tree->parent(n) ? tree->parent(n)->data() : 0) << std::endl;
			std::cout << "\t" << n->count(1) << std::endl;
			std::cout << "\t" << n->count(2) << std::endl;
			std::cout << "\t" << n->count(3) << std::endl;
			std::cout << "\t" << n->count(4) << std::endl;
			std::cout << "\t" << n->count(12) << std::endl;
			std::cout << "\t" << n->count(0) << std::endl;
			return 0;
		});
	}
}

SCENARIO_START

using namespace TEST_LAYER;

DESCRIBE("Tiq::Tree::LayersCollection", {
	DESCRIBE("init tree", {
		CollectionTree* tree;
		BEFORE_EACH({
			tree = new CollectionTree();
		});
		AFTER_EACH({
			delete tree;
		});

		IT("should return 0 for keys that are not found", {
			EXPECT(tree->get(10)).toBe(0);
			EXPECT(tree->size()).toBe(0);
		});

		IT("should set new value correctly", {
			tree->set(5, 10);

			EXPECT(tree->size()).toBe(1);
			EXPECT(tree->get(5)).toBe(10);
		});

		IT("should add value correctly", {
			tree->set(5, 1);
			tree->add(6, 2);

			tree->add(5, 3);

			EXPECT(tree->size()).toBe(2);
			EXPECT(tree->get(5)).toBe(4);
			EXPECT(tree->get(6)).toBe(2);
		});

		IT("should unset value correctly", {
			tree->set(5,1);
			tree->set(6,2);

			tree->unset(5);

			EXPECT(tree->get(5)).toBe(0);
			EXPECT(tree->get(6)).toBe(2);
			EXPECT(tree->size()).toBe(1);
		});

		IT("should merge trees correctly", {
			tree->set(5, 4);
			tree->set(6, 3);
			CollectionTree other;
			other.set(6, 10);
			other.set(7, 15);

			tree->merge(other);

			EXPECT(tree->size()).toBe(3);
			EXPECT(tree->get(5)).toBe(4);
			EXPECT(tree->get(6)).toBe(13);
			EXPECT(tree->get(7)).toBe(15);
		});

		IT("should calculate count correctly", {
			tree->set(2, 5);
			tree->set(1, 2);
			tree->set(4, 4);

			EXPECT(tree->count(1)).toBe(2);
			EXPECT(tree->count(2)).toBe(7);
			EXPECT(tree->count(3)).toBe(7);
			EXPECT(tree->count(4)).toBe(11);
		});

		DESCRIBE("added 20 values and removed 15", {
			BEFORE_EACH({
				for(int i=1;i<=10;i++) {
					tree->set(i,i*2);
					tree->set(i+10,i*3);
				}
				for(int i=1;i<=5;i++) {
					tree->unset(i);
					tree->unset(i+10);
					tree->unset(i+15);
				}
			});

			IT("should show correct values", {
				for(int i=6;i<=10;i++) {
					EXPECT(tree->get(i)).toBe(i*2);
				}
			});

			IT("should calculate counts correctly", {
				size_t res = 0;
				for(int i=6;i<=10;i++) {
					res += tree->get(i);
					EXPECT(tree->count(i)).toBe(res);
				}
				EXPECT(tree->count(10)).toBe(res);
			});

			IT("should clear the tree correctly", {
				tree->clear();

				EXPECT(tree->size()).toBe(0);
				EXPECT(tree->count(10)).toBe(0);
				EXPECT(tree->get(7)).toBe(0);
			});
		});
	});
});

DESCRIBE("Tiq::Tree::ValuesCollection", {
	DESCRIBE("init tree", {
		ValueTree* tree;
		BEFORE_EACH({
			tree = new ValueTree();
		});
		AFTER_EACH({
			delete tree;
		});

		IT("should be empty", {
			EXPECT(tree->size()).toBe(0);
			EXPECT(tree->has()).toBe(false);
		});

		DESCRIBE("add 10 items", {
			BEFORE_EACH({
				for (int i=1;i<=5;i++) {
					tree->set(i, i*2);
				}
				for (int i=11;i<=15;i++) {
					tree->set(i, i*2);
				}
			});

			IT("should contain 10 items", {
				EXPECT(tree->size()).toBe(10);
				for (int i=1;i<=5;i++) {
					EXPECT(tree->contains(i)).toBe(true);
				}
				for (int i=11;i<=15;i++) {
					EXPECT(tree->contains(i)).toBe(true);
				}
			});

			IT("should not contain non existing keys", {
				EXPECT(tree->contains(0)).toBe(false);
				EXPECT(tree->contains(7)).toBe(false);
				EXPECT(tree->contains(18)).toBe(false);
			});

			IT("should get currect data", {
				for (int i=1;i<=5;i++) {
					EXPECT(tree->has(i)).toBe(true);
					EXPECT(tree->get(i)).toBe(i*2);
				}
				for (int i=11;i<=15;i++) {
					EXPECT(tree->has(i)).toBe(true);
					EXPECT(tree->get(i)).toBe(i*2);
				}
				for (int i=6;i<=10;i++) {
					EXPECT(tree->has(i)).toBe(true);
					EXPECT(tree->get(i)).toBe(10);
				}
				for (int i=16;i<=50;i++) {
					EXPECT(tree->has(i)).toBe(true);
					EXPECT(tree->get(i)).toBe(30);
				}
				for (int i=-10;i<=0;i++) {
					EXPECT(tree->has(i)).toBe(false);
				}
				EXPECT([tree](){
					tree->get(0);
				}).toThrowError();
				EXPECT(tree->has()).toBe(true);
				EXPECT(tree->get()).toBe(30);
			});

			IT("should return correct keys", {
				EXPECT(tree->keys()).toBeIterableEqual({1,2,3,4,5,11,12,13,14,15});
			});

			DESCRIBE("unset some of the keys", {
				BEFORE_EACH({
					tree->unset(15);
					tree->unset(13);
					tree->unset(17);
				});

				IT("should get correctly", {
					EXPECT(tree->size()).toBe(8);
					EXPECT(tree->get()).toBe(28);
					EXPECT(tree->get(13)).toBe(24);
				});

				IT("should return correct keys", {
					EXPECT(tree->keys()).toBeIterableEqual({1,2,3,4,5,11,12,14});
				});
			});

			DESCRIBE("cut items", {
				BEFORE_EACH({
					tree->cut(4);
				});

				IT("should get correctly", {
					for (int i=1;i<=3;i++) {
						EXPECT(tree->get(i)).toBe(i*2);
					}
					EXPECT(tree->size()).toBe(3);
					EXPECT(tree->get(4)).toBe(6);
					EXPECT(tree->get()).toBe(6);
					EXPECT(tree->has(0)).toBe(false);
				});

				IT("should return correct keys", {
					EXPECT(tree->keys()).toBeIterableEqual({1,2,3});
				});
			});

			DESCRIBE("clear the tree", {
				BEFORE_EACH({
					tree->clear();
				});

				IT("should not have any items", {
					EXPECT(tree->size()).toBe(0);
					EXPECT(tree->has()).toBe(false);
					EXPECT(tree->has(1)).toBe(false);
					EXPECT([tree](){
						tree->get();
					}).toThrowError();
				});

				IT("should return empty array", {
					EXPECT(tree->keys().size()).toBe(0);
				});
			});
		});
	});
});

DESCRIBE("Tiq::Tree::LayerTree", {
	DESCRIBE("init the tree", {
		MyTree* tree;
		BEFORE_EACH({
			tree = new MyTree();
		});
		AFTER_EACH({
			delete tree;
		});

		DESCRIBE("Add 10 items with keys from 1 to 10 with different layer to the end of the tree", {
			BEFORE_EACH({
				for(int i=1;i<=10;i++){
					tree->insert(tree->end(), i, i);
				}
			});

			IT("should correctly assign layers", {
				auto b = tree->begin();
				while(!b->is_end()) {
					EXPECT(b->has_layer(b->data())).toBe(true);
					b = tree->find_next(b);
				}
			});

			IT("should count layer sizes correctly", {
				auto root = tree->root();
				for (int i=1;i<=10;i++) {
					EXPECT(root->count(i)).toBe(i);
				}
				EXPECT(root->count()).toBe(10);
			});
		});

		DESCRIBE("Add 20 items with complex layer ordering", {
			BEFORE_EACH({
				for(int i=1;i<=3;i++) {
					tree->insert(tree->find(bs_find(i)), i, 3);
				}
				for(int i=4;i<=6;i++) {
					tree->insert(tree->find(bs_find(i)), i, 2);
				}
				for(int i=7;i<=9;i++) {
					tree->insert(tree->find(bs_find(i)), i, 1);
				}
				for(int i=10;i<=11;i++) {
					tree->insert(tree->find(bs_find(i)), i, 10);
				}
				for(int i=12;i<=14;i++) {
					tree->insert(tree->find(bs_find(i)), i, 1);
				}
				for(int i=15;i<=17;i++) {
					tree->insert(tree->find(bs_find(i)), i, 3);
				}
				for(int i=18;i<=20;i++) {
					tree->insert(tree->find(bs_find(i)), i, 2);
				}
			});

			IT("should count layer sizes correctly", {
				EXPECT(tree->root()->count(1)).toBe(6);
				EXPECT(tree->root()->count(2)).toBe(12);
				EXPECT(tree->root()->count(3)).toBe(18);
				EXPECT(tree->root()->count(5)).toBe(18);
				EXPECT(tree->root()->count(10)).toBe(20);
				EXPECT(tree->root()->count(12)).toBe(20);
			});

			IT("should update node value", {
				auto node = tree->insert(tree->find(bs_find(20)), 30, 2);

				EXPECT(node->data()).toBe(30);
				EXPECT(tree->find(bs_find(20))->is_end()).toBe(true);
				EXPECT(tree->find(bs_find(30))->is_end()).toBe(false);
			});

			IT("should throw when erasing end node", {
				EXPECT([tree](){
					tree->erase(tree->find(bs_find(100)));
				}).toThrowError();
			});

			IT("should correctly find min item in layer", {
				EXPECT(tree->find_min(1)->data()).toBe(7);
				EXPECT(tree->find_min(2)->data()).toBe(4);
				EXPECT(tree->find_min(3)->data()).toBe(1);
				EXPECT(tree->find_min(4)->data()).toBe(1);
				EXPECT(tree->find_min(12)->data()).toBe(1);
				EXPECT(tree->find_min(0)->is_end()).toBe(true);
			});

			IT("should correctly find max item in layer", {
				EXPECT(tree->find_max(1)->data()).toBe(14);
				EXPECT(tree->find_max(2)->data()).toBe(20);
				EXPECT(tree->find_max(3)->data()).toBe(20);
				EXPECT(tree->find_max(4)->data()).toBe(20);
				EXPECT(tree->find_max(12)->data()).toBe(20);
				EXPECT(tree->find_max(0)->is_end()).toBe(true);
			});

			IT("should correctly find next item in layer", {
				std::vector<int> layers{1,2,3,4,12,0};
				std::vector<std::vector<int>> results{
					{7,8,9,12,13,14},
					{4,5,6,7,8,9,12,13,14,18,19,20},
					{1,2,3,4,5,6,7,8,9,12,13,14,15,16,17,18,19,20},
					{1,2,3,4,5,6,7,8,9,12,13,14,15,16,17,18,19,20},
					{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20},
					{}
				};

				int res_id=0;
				for(auto layer : layers) {
					std::vector<int> result;
					auto b = tree->left(tree->begin());
					do {
						b = tree->find_next(b, layer);
						if (b->is_end()) break;
						result.push_back(b->data());
					} while(true);
					EXPECT(result.size()).toBe(results[res_id].size());
					EXPECT(result).toBeIterableEqual(results[res_id++]);
				}
			});

			IT("should correctly find prev item in layer", {
				std::vector<int> layers{1,2,3,4,12,0};
				std::vector<std::vector<int>> results{
					{14,13,12,9,8,7},
					{20,19,18,14,13,12,9,8,7,6,5,4},
					{20,19,18,17,16,15,14,13,12,9,8,7,6,5,4,3,2,1},
					{20,19,18,17,16,15,14,13,12,9,8,7,6,5,4,3,2,1},
					{20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1},
					{}
				};

				int res_id=0;
				for(auto layer : layers) {
					std::vector<int> result;
					auto b = tree->end();
					do {
						b = tree->find_prev(b, layer);
						if (b->is_end()) break;
						result.push_back(b->data());
					} while(true);
					EXPECT(result).toBeIterableEqual(results[res_id++]);
				}
			});

			IT("should correctly find nth item in layer", {
				std::vector<int> layers{1,2,3,4,12,0};
				std::vector<int> layer_sizes{6, 12, 18, 18, 20, 0};
				std::vector<std::vector<int>> results{
					{7,8,9,12,13,14},
					{4,5,6,7,8,9,12,13,14,18,19,20},
					{1,2,3,4,5,6,7,8,9,12,13,14,15,16,17,18,19,20},
					{1,2,3,4,5,6,7,8,9,12,13,14,15,16,17,18,19,20},
					{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20},
					{}
				};

				int r=0;
				for (int size : layer_sizes) {
					auto &result = results[r];
					auto layer = layers[r++];
					for (int i=0;i<size;i++) {
						EXPECT(tree->find_nth(i, layer)->data()).toBe(result[i]);
					}
				}
			});

			IT("should correctly find item index", {
				std::vector<int> layers{1,2,3,4,12,0};
				std::vector<int> layers_size{6, 12, 18, 18, 20, 0};

				int l=0;
				for (auto layer : layers) {
					auto b = tree->find_min(layer);
					size_t ind = 0;
					while(!b->is_end()) {
						EXPECT(tree->find_index(b, layer)).toBe(ind++);
						b = tree->find_next(b, layer);
					}
					EXPECT(ind).toBe(layers_size[l++]);
				}
			});

			DESCRIBE("erase some items", {
				BEFORE_EACH({
					for(int i=4;i<=10;i++) {
						tree->erase(tree->find(bs_find(i)));
					}
					for(int i=20;i>=17;i--) {
						tree->erase(tree->find(bs_find(i)));
					}
				});

				IT("should correctly find min item in layer", {
					EXPECT(tree->find_min(1)->data()).toBe(12);
					EXPECT(tree->find_min(2)->data()).toBe(12);
					EXPECT(tree->find_min(3)->data()).toBe(1);
					EXPECT(tree->find_min(4)->data()).toBe(1);
					EXPECT(tree->find_min(12)->data()).toBe(1);
					EXPECT(tree->find_min(0)->is_end()).toBe(true);
				});

				IT("should correctly find max item in layer", {
					EXPECT(tree->find_max(1)->data()).toBe(14);
					EXPECT(tree->find_max(2)->data()).toBe(14);
					EXPECT(tree->find_max(3)->data()).toBe(16);
					EXPECT(tree->find_max(4)->data()).toBe(16);
					EXPECT(tree->find_max(12)->data()).toBe(16);
					EXPECT(tree->find_max(0)->is_end()).toBe(true);
				});

				IT("should correctly find next item in layer", {
					std::vector<int> layers{1,2,3,4,12,0};
					std::vector<std::vector<int>> results{
						{12,13,14},
						{12,13,14},
						{1,2,3,12,13,14,15,16},
						{1,2,3,12,13,14,15,16},
						{1,2,3,11,12,13,14,15,16},
						{}
					};

					int res_id=0;
					for(auto layer : layers) {
						std::vector<int> result;
						auto b = tree->left(tree->begin());
						do {
							b = tree->find_next(b, layer);
							if (b->is_end()) break;
							result.push_back(b->data());
						} while(true);
						EXPECT(result.size()).toBe(results[res_id].size());
						EXPECT(result).toBeIterableEqual(results[res_id++]);
					}
				});

				IT("should correctly find prev item in layer", {
					std::vector<int> layers{1,2,3,4,12,0};
					std::vector<std::vector<int>> results{
						{14,13,12},
						{14,13,12},
						{16,15,14,13,12,3,2,1},
						{16,15,14,13,12,3,2,1},
						{16,15,14,13,12,11,3,2,1},
						{}
					};

					int res_id=0;
					for(auto layer : layers) {
						std::vector<int> result;
						auto b = tree->end();
						do {
							b = tree->find_prev(b, layer);
							if (b->is_end()) break;
							result.push_back(b->data());
						} while(true);
						EXPECT(result).toBeIterableEqual(results[res_id++]);
					}
				});

				IT("should correctly find nth item in layer", {
					std::vector<int> layers{1,2,3,4,12,0};
					std::vector<int> layer_sizes{3, 3, 8, 8, 9, 0};
					std::vector<std::vector<int>> results{
						{12,13,14},
						{12,13,14},
						{1,2,3,12,13,14,15,16},
						{1,2,3,12,13,14,15,16},
						{1,2,3,11,12,13,14,15,16},
						{}
					};

					int r=0;
					for (int size : layer_sizes) {
						auto &result = results[r];
						auto layer = layers[r++];
						for (int i=0;i<size;i++) {
							EXPECT(tree->find_nth(i, layer)->data()).toBe(result[i]);
						}
					}
				});

				IT("should correctly find item index", {
					std::vector<int> layers{1,2,3,4,12,0};
					std::vector<int> layers_size{3, 3, 8, 8, 9, 0};

					int l=0;
					for (auto layer : layers) {
						auto b = tree->find_min(layer);
						size_t ind = 0;
						while(!b->is_end()) {
							EXPECT(tree->find_index(b, layer)).toBe(ind++);
							b = tree->find_next(b, layer);
						}
						EXPECT(ind).toBe(layers_size[l++]);
					}
				});
			});
		});
	});
});

SCENARIO_END

