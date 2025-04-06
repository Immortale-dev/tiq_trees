#ifndef TQ_TREE_ROPE_COUNT_TREE_H_
#define TQ_TREE_ROPE_COUNT_TREE_H_

#include <vector>
#include <cstdint>
#include <type_traits>

#include "tq_tree.h"
#include "tq_count_tree.h"
#include "tq_rope_interface.h"

namespace tiq::tree {
	template<typename T, typename Container = std::vector<T>>
	class RopeCountNode : public tiq::tree::CountNode<Container>, public RopeNodeInterface {
		template<typename _N, typename _A> friend class RopeCountTree;
		template<typename _D, typename _N> friend class RopeInterface;

		public:
			using value_list_t = Container;
			using value_t = T;
	};

	template<typename N, typename A = std::allocator<N>>
	class RopeCountTree : public tiq::tree::CountTree<N,A>, public tiq::tree::RopeInterface<RopeCountTree<N,A>,N> {
		friend class RopeInterface<RopeCountTree<N,A>,N>;
		public:
			using internal_node_ptr_t = InternalNode*;
			using node_ptr_t = N*;
			using node_list_t = std::vector<node_ptr_t>;
			using content_list_t = typename N::value_list_t;
			using value_list_iterator = typename content_list_t::iterator;

			explicit RopeCountTree(uint16_t value_size);

		protected:
			internal_node_ptr_t insert_(internal_node_ptr_t node) override;
			void calc_count(internal_node_ptr_t x) const override;
			size_t size_() const override;
	};
}

#include "tq_rope_count_tree.hpp"

#endif // TQ_TREE_ROPE_COUNT_TREE_H_
