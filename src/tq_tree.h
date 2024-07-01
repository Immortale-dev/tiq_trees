#ifndef TQ_TREE_H_
#define TQ_TREE_H_

#include <functional>
#include <type_traits>

namespace Tiq {
	template<class T, template<typename> class A = std::allocator>
	class Tree {
		public:
			class Node;
			using node_ptr_t = Node*;
			using const_node_ptr_t = typename std::add_const<node_ptr_t>::type;
			using comparator_fn_t = std::function<int(T&)>;

			class Node{
				friend Tree;

				public:
					const_node_ptr_t left() const { return left_; };
					const_node_ptr_t right() const { return right_; };
					const_node_ptr_t parent() const { return parent_; };
					T& data() { return data_; };
					bool is_end() const { return is_end_;  };

				private:
					node_ptr_t parent_ = nullptr;
					node_ptr_t left_ = nullptr;
					node_ptr_t right_ = nullptr;
					bool color_ = 0;
					bool is_end_ = 1;
					T data_ = {};
			};

			Tree();
			virtual ~Tree();

			const_node_ptr_t root() const;
			const_node_ptr_t begin() const;
			const_node_ptr_t end() const;

			const_node_ptr_t find(comparator_fn_t comp) const;
			const_node_ptr_t find(const_node_ptr_t node, comparator_fn_t comp) const;
			const_node_ptr_t find_min(const_node_ptr_t node = nullptr) const;
			const_node_ptr_t find_max(const_node_ptr_t node = nullptr) const;
			const_node_ptr_t find_next(const_node_ptr_t node) const;
			const_node_ptr_t find_prev(const_node_ptr_t node) const;
			const_node_ptr_t insert(const_node_ptr_t node, T data);
			const_node_ptr_t erase(const_node_ptr_t node);
			void clear();
			size_t size() const;

		private:
			void left_rotate(node_ptr_t x);
			void right_rotate(node_ptr_t x);
			void fix_delete(node_ptr_t x);
			void fix_insert(node_ptr_t x);
			void rb_transplant(node_ptr_t u, node_ptr_t v);

			node_ptr_t create_empty_node();
			void delete_node(node_ptr_t node);
			void dfs(node_ptr_t node, std::function<void(node_ptr_t)> fn);

			size_t count_;
			node_ptr_t root_;
			node_ptr_t begin_;
			node_ptr_t end_;

			A<Node> alloc_;
	};
}

#include "tq_tree.hpp"

#endif // TQ_TREE_H_
