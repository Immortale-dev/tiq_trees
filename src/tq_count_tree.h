#ifndef TQ_COUNT_TREE_H_
#define TQ_COUNT_TREE_H_

#include <type_traits>

#include "tq_tree.h"

namespace Tiq::Tree {
	template<class T>
	class CountNode : public Node<T> {
		template<class N, class A> friend class CountTree;
		public:
			using value_type = T;
			size_t count() const { return count_; };

		protected:
			size_t count_ = 0;
	};

	template<class N, class A = std::allocator<N>>
	class CountTree : public Tree<N,A> {
		public:
			using T = typename N::value_type;
			using node_ptr_t = InternalNode*;
			using const_node_ptr_t = N*;

		protected:
			node_ptr_t insert_(node_ptr_t node) override;
			node_ptr_t erase_(node_ptr_t node) override;

			void left_rotate(node_ptr_t x) override;
			void right_rotate(node_ptr_t x) override;
			void transplant(node_ptr_t u, node_ptr_t v) override;
			void calc_count(node_ptr_t x);

		private:
			void calc_upward(node_ptr_t x);
	};
}

#include "tq_count_tree.hpp"

#endif // TQ_COUNT_TREE_H_
