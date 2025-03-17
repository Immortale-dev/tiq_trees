#ifndef TQ_COUNT_TREE_H_
#define TQ_COUNT_TREE_H_

#include <type_traits>

#include "tq_tree.h"

namespace tiq::tree {
	template<class T>
	class CountNode : public Node<T> {
		template<class N, class A> friend class CountTree;
		public:
			using value_type = T;

			inline size_t count() const { return count_; };

		protected:
			size_t count_ = 0;
	};

	template<class N, class A = std::allocator<N>>
	class CountTree : public Tree<N,A> {
		public:
			using T = typename N::value_type;
			using internal_node_ptr_t = InternalNode*;
			using node_ptr_t = N*;

			node_ptr_t find_nth(size_t index) const;
			node_ptr_t find_nth(node_ptr_t node, size_t index) const;
			size_t find_index(node_ptr_t node, node_ptr_t parent = nullptr) const;

		protected:
			internal_node_ptr_t insert_(internal_node_ptr_t node) override;
			internal_node_ptr_t erase_(internal_node_ptr_t node) override;

			void left_rotate(internal_node_ptr_t x) override;
			void right_rotate(internal_node_ptr_t x) override;
			void transplant(internal_node_ptr_t u, internal_node_ptr_t v) override;
			virtual inline void calc_count(internal_node_ptr_t x);

		private:
			void calc_upward(internal_node_ptr_t x);
	};
} // namespace tiq::tree

#include "tq_count_tree.hpp"

#endif // TQ_COUNT_TREE_H_
