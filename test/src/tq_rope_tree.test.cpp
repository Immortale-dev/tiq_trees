#include "tq_rope_tree.h"

namespace TEST_ROPE_NS {
	using MyNode = tiq::tree::RopeNode<int, std::vector<int>>;
	using MyTree = tiq::tree::RopeTree<MyNode>;

	std::vector<int> get_rope_content(MyTree &r) {
		std::vector<int> result;
		auto b = r.begin();
		while(!b->is_end()) {
			result.insert(result.end(), b->data().begin(), b->data().end());
			b = r.find_next(b);
		}
		return result;
	}

	bool validate(MyTree &r, size_t size) {
		auto b = r.begin();
		while (!b->is_end()) {
			if (b->size() > size*2 || b->size() < size/2) return false;
			b = r.find_next(b);
		}
		return true;
	}

	void tree_max_height(MyTree *tree, MyNode *node, int &max_height, int current) {
		if (node->is_end()) {
			max_height = std::max(max_height, current+1);
		} else {
			tree_max_height(tree, tree->left(node), max_height, current+1);
			tree_max_height(tree, tree->right(node), max_height, current+1);
		}
	}

	void inorder_print(MyTree *tree, MyNode *node, int max_height, int current) {
		if (node->is_end()) return;
		inorder_print(tree, tree->left(node), max_height, current+1);
		for (int i=0;i<max_height-current+1;i++) std::cout << "        ";
		std::cout << "[" << (node->size()) << "](" << node->count() << ")" << std::endl;
		inorder_print(tree, tree->right(node), max_height, current+1);
	}

	void print_tree(MyTree *tree) {
		int max_height = 0;
		tree_max_height(tree, tree->root(), max_height, 0);
		inorder_print(tree, tree->root(), max_height, 0);
	}
}

SCENARIO_START

using namespace TEST_ROPE_NS;

DESCRIBE("tiq::tree::detail::Rope", {
	IT("should construct the rope", {
		MyTree rope(100);

		EXPECT(rope.size()).toBe(0);
	});

	DESCRIBE("Empty rope with chunk size 10 is created", {
		MyTree *rope;
		BEFORE_EACH({
			rope = new MyTree(10);
		});
		AFTER_EACH({
			delete rope;
		});

		IT("should add small content to the end of the rope", {
			std::vector<int> s{1,2,3,4,5};
			rope->insert(rope->end(), s);

			EXPECT(rope->size()).toBe(5);
			EXPECT(get_rope_content(*rope)).toBeIterableEqual(s);
			EXPECT(validate(*rope, 10)).toBe(true);
		});

		IT("should add big content to the end of the rope", {
			std::vector<int> s;
			for (int i=0;i<27;i++) {
				s.push_back(i+10);
			}
			rope->insert(rope->end(), s);

			int nodes_cnt = 0;
			auto b = rope->find_min();
			auto e = rope->find_next(rope->find_max());
			while(b != e) {
				nodes_cnt++;
				b = rope->find_next(b);
			}

			EXPECT(rope->size()).toBe(s.size());
			EXPECT(nodes_cnt).toBe(3);
			EXPECT(s).toBeIterableEqual(get_rope_content(*rope));
			EXPECT(validate(*rope, 10)).toBe(true);
		});

		IT("should add smaller content to the end of the rope", {
			std::vector<int> s;
			for (int i=0;i<21;i++) {
				s.push_back(i+10);
			}
			rope->insert(rope->end(), s);

			int nodes_cnt = 0;
			auto b = rope->find_min();
			auto e = rope->find_next(rope->find_max());
			while(b != e) {
				nodes_cnt++;
				b = rope->find_next(b);
			}

			EXPECT(rope->size()).toBe(s.size());
			EXPECT(nodes_cnt).toBe(2);
			EXPECT(s).toBeIterableEqual(get_rope_content(*rope));
			EXPECT(validate(*rope, 10)).toBe(true);
		});
	});

	DESCRIBE("A rope with a content created", {
		MyTree *rope;
		std::vector<int> s;

		BEFORE_EACH({
			s.clear();
			for (int i=0;i<30;i++) {
				s.push_back(i+1);
			}
			rope = new MyTree(10);
			rope->insert(rope->begin(), s);
		});
		AFTER_EACH({
			delete rope;
		});

		IT("should add more content to the end of the rope", {
			std::vector<int> s1;
			for (int i=0;i<26;i++) {
				s1.push_back(101+i);
			}
			auto n = rope->find_max();
			std::vector<int> data = n->data();
			data.insert(data.end(), s1.begin(), s1.end());

			rope->insert(n, data);

			std::vector<int> result = s;
			result.insert(result.end(), s1.begin(), s1.end());
			EXPECT(rope->size()).toBe(s.size()+s1.size());
			EXPECT(result).toBeIterableEqual(get_rope_content(*rope));
			EXPECT(validate(*rope, 10)).toBe(true);
		});

		IT("should add more content to the beginning of the rope", {
			std::vector<int> s1;
			for (int i=0;i<26;i++) {
				s1.push_back(101+i);
			}
			auto n = rope->begin();
			std::vector<int> data = n->data();
			data.insert(data.begin(), s1.begin(), s1.end());

			rope->insert(rope->begin(), data);

			std::vector<int> result = s;
			result.insert(result.begin(), s1.begin(), s1.end());
			EXPECT(rope->size()).toBe(s.size()+s1.size());
			EXPECT(result).toBeIterableEqual(get_rope_content(*rope));
			EXPECT(validate(*rope, 10)).toBe(true);
		});

		IT("should add a lot of content to the middle of the rope", {
			std::vector<int> s1;
			for (int i=0;i<30;i++) {
				s1.push_back(201+i);
			}
			auto n = rope->find_next(rope->begin());
			std::vector<int> data = n->data();
			data.insert(data.end(), s1.begin(), s1.end());

			rope->insert(n, data);

			EXPECT(rope->size()).toBe(s.size() + s1.size());
			EXPECT(validate(*rope, 10)).toBe(true);
 		});

		IT("should add a bit of content to the middle of the rope", {
			std::vector<int> s1 {55,56,57};
			auto n = rope->find_next(rope->begin());
			std::vector<int> data = n->data();
			data.insert(data.end(), s1.begin(), s1.end());

			rope->insert(n, data);

			EXPECT(rope->size()).toBe(s.size() + s1.size());
			EXPECT(validate(*rope, 10)).toBe(true);
		});
	});
});

SCENARIO_END
