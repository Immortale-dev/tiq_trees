#ifndef TQ_ENGINE_DETAIL_ROPE_H_
#define TQ_ENGINE_DETAIL_ROPE_H_

#include <vector>
#include <cstdint>
#include <type_traits>

#include "tq_tree.h"
#include "tq_count_tree.h"

namespace tiq::tree {
	template<typename T, typename Container = std::vector<T>>
	class RopeNode : public tiq::tree::CountNode<Container> {
		template<typename _N, typename _A> friend class RopeTree;

		public:
			using value_list_t = Container;
			using value_t = T;

			inline uint16_t size() const { return size_; }

		protected:
			uint16_t size_ = 0;
	};

	template<typename N, typename A = std::allocator<N>>
	class RopeTree : public tiq::tree::CountTree<N> {
		public:
			using internal_node_ptr_t = tiq::tree::InternalNode*;
			using node_ptr_t = N*;
			using node_list_t = std::vector<node_ptr_t>;
			using content_list_t = typename N::value_list_t;
			using value_list_iterator = typename content_list_t::iterator;

			explicit RopeTree(uint16_t value_size);

		protected:
			internal_node_ptr_t insert_(internal_node_ptr_t node) override;
			void calc_count(internal_node_ptr_t x) override;
			size_t size_() const override;

			virtual void calc_size(internal_node_ptr_t node) const;
			virtual value_list_iterator find_offset_item(content_list_t& container, value_list_iterator b, int16_t offset) const;
			virtual void relax(node_ptr_t begin, node_ptr_t end);

			uint16_t value_size_;
			uint16_t value_min_size_;
			uint16_t value_max_size_;
	};
}

#include "tq_rope_tree.hpp"

#endif // TQ_ENGINE_DETAIL_ROPE_H_
