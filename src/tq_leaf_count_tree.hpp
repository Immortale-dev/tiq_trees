#include <stdexcept>

#include "tq_leaf_tree.h"

template<class N, class A>
typename tiq::tree::LeafCountTree<N,A>::node_ptr_t tiq::tree::LeafCountTree<N,A>::find_nth(size_t index) const
{
	return find_nth(this->root(), index);
}

template<class N, class A>
typename tiq::tree::LeafCountTree<N,A>::node_ptr_t tiq::tree::LeafCountTree<N,A>::find_nth(node_ptr_t node, size_t index) const
{
	if (node->is_end()) {
		return node;
	}
	if (node->count() <= index) {
		return this->right(this->find_max(node));
	}
	while(true) {
		if (node->is_leaf() && index == 0) return node;
		size_t left_count = this->left(node)->count();
		if (left_count <= index) {
			index -= left_count;
			node = this->right(node);
		} else {
			node = this->left(node);
		}
	}
}

template<class N, class A>
size_t tiq::tree::LeafCountTree<N,A>::find_index(node_ptr_t node, node_ptr_t parent) const
{
	if (!node->is_leaf()) {
		throw std::logic_error("node must be a leaf node");
	}
	size_t count = 0;
	if (!parent) {
		parent = this->root();
	}
	while(node != parent) {
		auto par = this->parent(node);
		if (!par) {
			throw std::logic_error("node is not child of parent");
		}
		if (this->right(par) == node) {
			count += this->left(par)->count();
		}
		node = par;
	}
	return count;
}

template<class N, class A>
typename tiq::tree::LeafCountTree<N,A>::internal_node_ptr_t tiq::tree::LeafCountTree<N,A>::insert_(internal_node_ptr_t node)
{
	internal_node_ptr_t n = LeafTree<N,A>::insert_(node);
	calc_upward(n);
	return n;
}

template<class N, class A>
typename tiq::tree::LeafCountTree<N,A>::internal_node_ptr_t tiq::tree::LeafCountTree<N,A>::erase_(internal_node_ptr_t node)
{
	internal_node_ptr_t n = LeafTree<N,A>::erase_(node);
	calc_upward(n);
	return n;
}

template<class N, class A>
void tiq::tree::LeafCountTree<N,A>::left_rotate(internal_node_ptr_t x)
{
	LeafTree<N,A>::left_rotate(x);
	calc_count(x);
	calc_count(this->parent_(x));
}

template<class N, class A>
void tiq::tree::LeafCountTree<N,A>::right_rotate(internal_node_ptr_t x)
{
	LeafTree<N,A>::right_rotate(x);
	calc_count(x);
	calc_count(this->parent_(x));
}

template<class N, class A>
void tiq::tree::LeafCountTree<N,A>::transplant(internal_node_ptr_t u, internal_node_ptr_t v)
{
	LeafTree<N,A>::transplant(u, v);
	if (!v->is_end()) {
		calc_count(v);
	}
}

template<class N, class A>
void tiq::tree::LeafCountTree<N,A>::calc_count(internal_node_ptr_t x)
{
	auto node = this->to_public_node(x);
	if (node->is_end()) {
		node->count_ = 0;
		return;
	}
	if (node->is_leaf()) {
		node->count_ = 1;
		return;
	}
	node->count_ = this->to_public_node(this->left_(x))->count_ + this->to_public_node(this->right_(x))->count_;
}

template<class N, class A>
void tiq::tree::LeafCountTree<N,A>::calc_upward(internal_node_ptr_t x)
{
	do {
		calc_count(x);
	} while((x = this->parent_(x)));
}
