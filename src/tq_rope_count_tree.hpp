#include <algorithm>
#include <iterator>

template<typename N, typename A>
tiq::tree::RopeCountTree<N,A>::RopeCountTree(uint16_t value_size) : RopeInterface<RopeCountTree<N,A>,N>(value_size) {}

template<typename N, typename A>
typename tiq::tree::RopeCountTree<N,A>::internal_node_ptr_t tiq::tree::RopeCountTree<N,A>::insert_(internal_node_ptr_t node)
{
	this->calc_size(node);
	tiq::tree::CountTree<N,A>::insert_(node);
	auto n = this->to_public_node(node);
	if (n->size() > this->value_max_size_ || (n->size() < this->value_min_size_ && this->size() > n->size())) {
		this->relax(n, this->find_next(n));
	}
	return node;
}

template<typename N, typename A>
void tiq::tree::RopeCountTree<N,A>::calc_count(internal_node_ptr_t x) const
{
	if (x->is_end()) return;
	node_ptr_t node = this->to_public_node(x);
	node->count_ = this->left(node)->count() + this->right(node)->count() + node->size();
}

template<typename N, typename A>
size_t tiq::tree::RopeCountTree<N,A>::size_() const
{
	return this->root()->count();
}
