#ifndef TQ_ROPE_INTERFACE_H
#define TQ_ROPE_INTERFACE_H

namespace tiq::tree {

	class RopeNodeInterface {
		template<typename _D, typename _N> friend class RopeInterface;

		public:
			inline uint16_t size() const { return size_; }

		protected:
			uint16_t size_ = 0;
	};

	template<typename D, typename N>
	class RopeInterface {
		public:
			using internal_node_ptr_t = InternalNode*;
			using node_ptr_t = N*;
			using node_list_t = std::vector<node_ptr_t>;
			using content_list_t = typename N::value_list_t;
			using value_list_iterator = typename content_list_t::iterator;

		protected:
			RopeInterface(uint16_t value_size);

			virtual void calc_size(internal_node_ptr_t node) const;
			virtual value_list_iterator find_offset_item(content_list_t& container, value_list_iterator b, int16_t offset) const;
			virtual void relax(node_ptr_t begin, node_ptr_t end);

			uint16_t value_size_;
			uint16_t value_min_size_;
			uint16_t value_max_size_;
	};

} // namespace tiq::tree

#include "tq_rope_interface.hpp"

#endif // TQ_ROPE_INTERFACE_H
