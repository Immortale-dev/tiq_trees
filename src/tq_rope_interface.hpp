#include <algorithm>
#include <iterator>
#include <cassert>

template<typename D, typename N>
tiq::tree::RopeInterface<D,N>::RopeInterface(uint16_t value_size) : value_size_(value_size), value_min_size_(value_size/2), value_max_size_(value_size*2) {}

template<typename D, typename N>
void tiq::tree::RopeInterface<D,N>::calc_size(internal_node_ptr_t node) const
{
	auto n = static_cast<node_ptr_t>(node);
	if (!n->has_data()) return;
	n->size_ = n->data_->size();
}

template<typename D, typename N>
uint16_t tiq::tree::RopeInterface<D,N>::find_offset_item(
	[[maybe_unused]] const value_t* data,
	uint16_t size,
	uint16_t current,
	int16_t offset
) const
{
	return static_cast<uint16_t>(std::clamp<int16_t>(static_cast<int16_t>(current) + offset, static_cast<int16_t>(0), static_cast<int16_t>(size)));
}

template<typename D, typename N>
void tiq::tree::RopeInterface<D,N>::relax(node_ptr_t begin, node_ptr_t end)
{
	auto this_ = static_cast<D*>(this);
	auto n = begin;
	size_t total_size = 0;
	while (n != end) {
		total_size += n->size();
		n = this_->find_next(n);
	}
	if (total_size < value_min_size_) {
		if (!end->is_end()) {
			end = this_->find_next(end);
		} else if(this_->begin() != begin) {
			begin = this_->find_prev(begin);
		}
	}

	node_list_t nodes;
	n = begin;
	total_size = 0;
	while (n != end) {
		nodes.push_back(n);
		total_size += n->size();
		n = this_->find_next(n);
	}
	content_list_t content;
	for (auto n : nodes) {
		content.insert(content.end(), std::make_move_iterator(n->data_->begin()), std::make_move_iterator(n->data_->end()));
		n->data_->clear();
	}

	size_t rest = total_size % value_size_;
	size_t cnt = total_size ? ((total_size - 1)/value_size_ + 1) : 0;
	bool take_rest = (rest > 0 && rest < value_min_size_);
	if (take_rest > 0 && cnt > 1) {
		cnt--;
	}

	if (nodes.size() > cnt) {
		// remove nodes that are going to be empty
		for (size_t j=cnt;j<nodes.size();j++) {
			this_->erase(nodes[j]);
		}
		nodes.resize(cnt);
	}

	assert(content.size() < (1<<15));

	size_t i=0;
	uint16_t ib, ie = 0;
	uint16_t isz = static_cast<uint16_t>(content.size());
	for (;i<std::min(cnt,nodes.size());i++) {
		size_t take = value_size_ + (take_rest ? rest : 0);
		take_rest = false;
		ib = ie;
		ie = find_offset_item(content.data(), isz, ie, take);
		nodes[i]->data_ = content_list_t(std::make_move_iterator(content.begin() + ib), std::make_move_iterator(content.begin() + ie));
		this_->insert_(nodes[i]);
	}

	if (i == cnt) return;

	n = this_->before(end);
	for (;i<cnt;i++) {
		ib = ie;
		ie = find_offset_item(content.data(), isz, ie, value_size_);
		n->data_ = content_list_t(std::make_move_iterator(content.begin() + ib), std::make_move_iterator(content.begin() + ie));
		this_->insert_(n);
		n = this_->after(n);
	}
}
