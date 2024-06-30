#ifndef TQ_TREE_H_
#define TQ_TREE_H_

#include <functional>
#include <memory>
#include <type_traits>

namespace Tiq {
	template<class T>
	class Tree {
		public:
			class Node;
			using node_ptr_t = Node*;
			using const_node_ptr_t = std::add_const<node_ptr_t>::type;
			using comparator_fn_t = std::function<int(T&)>;

			class Node{
				friend Tree;

				public:
					const_node_ptr_t get_left() const;
					const_node_ptr_t get_right() const;
					const_node_ptr_t get_parent() const;
					T& get_data() const;
					bool is_end() const;

				private:
					T data;
					node_ptr_t parent_;
					node_ptr_t left_;
					node_ptr_t right_;
					bool color_;
					bool is_end_;
			};

			Tree();
			~Tree();

			const_node_ptr_t root() const;
			const_node_ptr_t begin() const;
			const_node_ptr_t end() const;

			const_node_ptr_t find(comparator_fn_t comp) const;
			const_node_ptr_t find(const_node_ptr_t node, comparator_fn_t comp) const;
			const_node_ptr_t find_min(const_node_ptr_t node) const;
			const_node_ptr_t find_max(const_node_ptr_t node) const;
			const_node_ptr_t find_next(const_node_ptr_t node) const;
			const_node_ptr_t find_prev(const_node_ptr_t node) const;
			const_node_ptr_t insert(const_node_ptr_t node, T data) const;
			const_node_ptr_t insert(const_node_ptr_t node, T&& data) const;
			const_node_ptr_t erase(const_node_ptr_t node) const;
			const_node_ptr_t begin() const;
			const_node_ptr_t end() const;
			void clear();
			size_t size() const;

		private:
			void left_rotate(node_ptr_t x);
			void right_rotate(node_ptr_t x);
			void fix_delete(node_ptr_t x);
			void fix_insert(node_ptr_t x);
			void rb_transplant(node_ptr_t u, note_ptr_t v);

			node_ptr_t create_empty_node();
			void dfs(node_ptr_t node, std::function<void(node_ptr_t)> fn);

			size_t count_;
			node_ptr_t root_;
			node_ptr_t first_;
			node_ptr_t last_;
	};
}

#include "tq_tree.hpp"

#endif // TQ_TREE_H_
