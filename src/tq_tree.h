#ifndef TQ_TREE_H_
#define TQ_TREE_H_

#include <functional>
#include <type_traits>

namespace Tiq::Tree {
	class InternalNode {
		template<class N, class A> friend class Tree;
		using node_ptr_t = InternalNode*;

		public:
			virtual ~InternalNode(){}
			bool is_end() const { return is_end_; }

		protected:
			node_ptr_t parent_ = nullptr;
			node_ptr_t left_ = nullptr;
			node_ptr_t right_ = nullptr;

		private:
			bool color_ = 0;
			bool is_end_ = 1;
	};

	template<class T>
	class Node : public InternalNode {
		public:
			using value_type = T;

			T& data() { return data_; }

		private:
			T data_ = {};
	};

	template<class N, class A = std::allocator<N>>
	class Tree {
		public:
			using T = typename N::value_type;
			using node_ptr_t = InternalNode*;
			using const_node_ptr_t = N*;
			using comparator_fn_t = std::function<int(T&)>;

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
			const_node_ptr_t parent(const_node_ptr_t node) const;
			const_node_ptr_t left(const_node_ptr_t node) const;
			const_node_ptr_t right(const_node_ptr_t node) const;
			const_node_ptr_t insert(const_node_ptr_t node, T data);
			const_node_ptr_t erase(const_node_ptr_t node);
			void clear();
			size_t size() const;

		protected:
			virtual node_ptr_t find_(node_ptr_t node, comparator_fn_t comp) const;
			virtual node_ptr_t find_min_(node_ptr_t node) const;
			virtual node_ptr_t find_max_(node_ptr_t node) const;
			virtual node_ptr_t insert_(node_ptr_t node);
			virtual node_ptr_t erase_(node_ptr_t node);

			virtual void left_rotate(node_ptr_t x);
			virtual void right_rotate(node_ptr_t x);
			virtual void transplant(node_ptr_t u, node_ptr_t v);

			node_ptr_t left_(node_ptr_t);
			node_ptr_t right_(node_ptr_t);
			node_ptr_t parent_(node_ptr_t);

			const_node_ptr_t to_public_node(node_ptr_t node) const;
			node_ptr_t to_internal_node(const_node_ptr_t node) const;
			node_ptr_t create_empty_node();
			void delete_node(node_ptr_t node);

		private:
			void fix_delete(node_ptr_t x);
			void fix_insert(node_ptr_t x);

			void dfs(node_ptr_t node, std::function<void(node_ptr_t)> fn);

			size_t count_;
			node_ptr_t root_;
			node_ptr_t begin_;
			node_ptr_t end_;

			A alloc_;
	};
} // namespace Tiq::Tree

#include "tq_tree.hpp"

#endif // TQ_TREE_H_
