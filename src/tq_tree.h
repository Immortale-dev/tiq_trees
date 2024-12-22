#ifndef TQ_TREE_H_
#define TQ_TREE_H_

#include <functional>
#include <type_traits>
#include <optional>

namespace tiq::tree {
	class InternalNode {
		template<class N, class A> friend class Tree;
		using internal_node_ptr_t = InternalNode*;

		public:
			virtual ~InternalNode(){}
			bool is_end() const { return is_end_; }

		protected:
			internal_node_ptr_t parent_ = nullptr;
			internal_node_ptr_t left_ = nullptr;
			internal_node_ptr_t right_ = nullptr;

		private:
			bool color_ = 0;
			bool is_end_ = 1;
	};

	template<class T>
	class Node : public InternalNode {
		template<class _N, class _A> friend class Tree;
		public:
			using value_type = T;

			const T& data() const { return data_.value(); }

		protected:
			std::optional<T> data_;
	};

	template<class N, class A = std::allocator<N>>
	class Tree {
		public:
			using T = typename N::value_type;
			using internal_node_ptr_t = InternalNode*;
			using node_ptr_t = N*;
			using comparator_fn_t = std::function<int(const node_ptr_t node)>;

			Tree();
			virtual ~Tree();
			Tree(const Tree&) = delete;
			Tree& operator=(const Tree&) = delete;

			node_ptr_t root() const;
			node_ptr_t begin() const;
			node_ptr_t end() const;

			node_ptr_t find(comparator_fn_t comp) const;
			node_ptr_t find(node_ptr_t node, comparator_fn_t comp) const;
			node_ptr_t find_min(node_ptr_t node = nullptr) const;
			node_ptr_t find_max(node_ptr_t node = nullptr) const;
			node_ptr_t find_next(node_ptr_t node) const;
			node_ptr_t find_prev(node_ptr_t node) const;
			node_ptr_t parent(node_ptr_t node) const;
			node_ptr_t left(node_ptr_t node) const;
			node_ptr_t right(node_ptr_t node) const;
			node_ptr_t before(node_ptr_t node = nullptr) const;
			node_ptr_t after(node_ptr_t node = nullptr) const;
			node_ptr_t insert(node_ptr_t node, T data);
			node_ptr_t erase(node_ptr_t node);
			void clear();
			size_t size() const;

		protected:
			virtual internal_node_ptr_t find_(internal_node_ptr_t node, comparator_fn_t comp) const;
			virtual internal_node_ptr_t find_min_(internal_node_ptr_t node) const;
			virtual internal_node_ptr_t find_max_(internal_node_ptr_t node) const;
			virtual internal_node_ptr_t insert_(internal_node_ptr_t node);
			virtual internal_node_ptr_t erase_(internal_node_ptr_t node);
			virtual size_t size_() const;

			virtual void left_rotate(internal_node_ptr_t x);
			virtual void right_rotate(internal_node_ptr_t x);
			virtual void transplant(internal_node_ptr_t u, internal_node_ptr_t v);

			internal_node_ptr_t left_(internal_node_ptr_t) const;
			internal_node_ptr_t right_(internal_node_ptr_t) const;
			internal_node_ptr_t parent_(internal_node_ptr_t) const;

			node_ptr_t to_public_node(internal_node_ptr_t node) const;
			internal_node_ptr_t to_internal_node(node_ptr_t node) const;
			internal_node_ptr_t create_empty_node();
			void delete_node(internal_node_ptr_t node);

		private:
			void fix_delete(internal_node_ptr_t x);
			void fix_insert(internal_node_ptr_t x);

			void dfs(internal_node_ptr_t node, std::function<void(internal_node_ptr_t)> fn);

			size_t count_;
			internal_node_ptr_t root_;
			internal_node_ptr_t begin_;
			internal_node_ptr_t end_;

			A alloc_;
	};
} // namespace tiq::tree

#include "tq_tree.hpp"

#endif // TQ_TREE_H_
