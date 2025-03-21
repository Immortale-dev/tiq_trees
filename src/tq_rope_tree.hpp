#include <algorithm>
#include <iterator>

template<typename N, typename A>
tiq::tree::RopeTree<N,A>::RopeTree(uint16_t value_size) : value_size_(value_size), value_min_size_(value_size/2), value_max_size_(value_size*2) {}

template<typename N, typename A>
typename tiq::tree::RopeTree<N,A>::internal_node_ptr_t tiq::tree::RopeTree<N,A>::insert_(internal_node_ptr_t node)
{
	calc_size(node);
	tiq::tree::CountTree<N,A>::insert_(node);
	auto n = this->to_public_node(node);
	if (n->size() > value_max_size_ || (n->size() < value_min_size_ && this->size() > n->size())) {
		relax(n, this->find_next(n));
	}
	return node;
}

template<typename N, typename A>
void tiq::tree::RopeTree<N,A>::calc_count(internal_node_ptr_t x)
{
	if (x->is_end()) return;
	node_ptr_t node = this->to_public_node(x);
	node->count_ = this->left(node)->count() + this->right(node)->count() + node->size();
}

template<typename N, typename A>
size_t tiq::tree::RopeTree<N,A>::size_() const
{
	return this->root()->count();
}

template<typename N, typename A>
void tiq::tree::RopeTree<N,A>::calc_size(internal_node_ptr_t node) const
{
	node_ptr_t n = this->to_public_node(node);
	if (!n->data_) return;
	n->size_ = n->data_->size();
}

template<typename N, typename A>
typename tiq::tree::RopeTree<N,A>::value_list_iterator tiq::tree::RopeTree<N,A>::find_offset_item([[maybe_unused]] content_list_t& container, value_list_iterator b, int16_t offset) const
{
	if (offset > 0) {
		if (std::distance(b, container.cend()) <= offset) return container.end();
		return std::next(b, offset);
	}
	if (std::distance(container.cbegin(), b) < -offset) return std::prev(container.begin());
	return std::prev(b, -offset);
}

template<typename N, typename A>
void tiq::tree::RopeTree<N,A>::relax(node_ptr_t begin, node_ptr_t end)
{
	auto n = begin;
	size_t total_size = 0;
	while (n != end) {
		total_size += n->size();
		n = this->find_next(n);
	}
	if (total_size < value_min_size_) {
		if (!end->is_end()) {
			end = this->find_next(end);
		} else if(this->begin() != begin) {
			begin = this->find_prev(begin);
		}
	}

	node_list_t nodes;
	n = begin;
	total_size = 0;
	while (n != end) {
		nodes.push_back(n);
		total_size += n->size();
		n = this->find_next(n);
	}
	content_list_t content;
	for (auto n : nodes) {
		content.insert(content.end(), std::make_move_iterator(n->data().begin()), std::make_move_iterator(n->data().end()));
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
			this->erase(nodes[j]);
		}
		nodes.resize(cnt);
	}

	size_t i=0;
	value_list_iterator b,e = content.begin();
	for (;i<std::min(cnt,nodes.size());i++) {
		size_t take = value_size_ + (take_rest ? rest : 0);
		take_rest = false;
		b = e;
		e = find_offset_item(content, e, take);
		this->insert(nodes[i], content_list_t(std::make_move_iterator(b), std::make_move_iterator(e)));
	}

	if (i == cnt) return;

	n = this->before(end);
	for (;i<cnt;i++) {
		b = e;
		e = find_offset_item(content, e, value_size_);
		this->insert(n, content_list_t(b, e));
		n = this->after(n);
	}
}
