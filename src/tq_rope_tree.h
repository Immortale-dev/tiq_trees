#ifndef TQ_TREE_ROPE_TREE_H_
#define TQ_TREE_ROPE_TREE_H_

#include "tq_tree"
#include "tq_rope_interface.h"

namespace tiq::tree {

	template<typename T, typename Container = std::vector<T>>
	class RopeCountNode : public tiq::tree::CountNode<Container>, public RopeNodeInterface {
		template<typename _N, typename _A> friend class RopeTree;
		template<typename _D, typename _N> friend class RopeInterface;

		public:
			using value_list_t = Container;
			using value_t = T;
	};

	template<typename N, typename A = std::allocator<N>>
	class RopeTree : public tiq::tree::Tree<N,A>, public tiq::tree::RopeInterface<RopeCountTree<N,A>,N> {
		public:
			internal_node_ptr_t insert_(internal_node_ptr_t node) override;
	};
}

#endif // TQ_TREE_ROPE_TREE_H_

