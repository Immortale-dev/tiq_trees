#ifndef TQ_LEAF_TREE_H_
#define TQ_LEAF_TREE_H_

#include "tq_tree.h"

namespace tiq::tree {
	template<class T>
	class LeafNode : public Node<T> {
		template<class _N, class _A> friend class LeafTree;
		public:
			using value_type = T;

			bool is_leaf() const { return leaf_; }

		protected:
			bool leaf_ = 0;
	};

	template<class N, class A = std::allocator<N>>
	class LeafTree : public Tree<N,A> {
		public:
			using T = typename N::value_type;
			using internal_node_ptr_t = InternalNode*;
			using node_ptr_t = N*;

			node_ptr_t find_next_leaf(node_ptr_t node) const;
			node_ptr_t find_prev_leaf(node_ptr_t node) const;

		protected:
			internal_node_ptr_t insert_(internal_node_ptr_t node) override;
			internal_node_ptr_t erase_(internal_node_ptr_t node) override;
	};
} // namespace tiq::tree

#include "tq_leaf_tree.hpp"

#endif // TQ_LEAF_TREE_H_
