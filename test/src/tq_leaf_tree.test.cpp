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

		DESCRIBE("Add 10 items with keys from 1 to 10 to the end of the tree", {
			BEFORE_EACH({
				for(int i=1;i<=10;i++){
					tree->insert(tree->end(), i);
				}
			});

			IT("should ...", {
				// TODO: continue 
				TEST_SUCCEED();
			});
		});
	});
});

SCENARIO_END
