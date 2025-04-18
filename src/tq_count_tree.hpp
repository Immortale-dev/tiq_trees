#include <stdexcept>

#include "tq_tree.h"

template<class N, class A>
typename tiq::tree::CountTree<N,A>::node_ptr_t tiq::tree::CountTree<N,A>::find_nth(node_ptr_t node, size_t index) const
{
	if (node->is_end()) {
		return node;
	}
	if (node->count() <= index) {
		return this->right(this->find_max(node));
	}
	while(true) {
		size_t left_count = this->left(node)->count();
		if (left_count == index) {
			return node;
		}
		if (left_count < index) {
			index -= left_count + 1; // + current node.
			node = this->right(node);
		} else {
			node = this->left(node);
		}
	}
}

template<class N, class A>
typename tiq::tree::CountTree<N,A>::node_ptr_t tiq::tree::CountTree<N,A>::find_nth(size_t index) const
{
	return find_nth(this->root(), index);
}

template<class N, class A>
size_t tiq::tree::CountTree<N,A>::find_index(node_ptr_t node, node_ptr_t parent) const
{
	size_t count = 0;
	if (!node->is_end()) {
		count += this->left(node)->count();
	}
	if (!parent) {
		parent = this->root();
	}
	while(node != parent) {
		auto par = this->parent(node);
		if (!par) {
			throw std::logic_error("node is not child of parent.");
		}
		if (this->right(par) == node) {
			count += this->left(par)->count() + 1;
		}
		node = par;
	}
	return count;
}

template<class N, class A>
typename tiq::tree::CountTree<N,A>::internal_node_ptr_t tiq::tree::CountTree<N,A>::insert_(internal_node_ptr_t node)
{
	internal_node_ptr_t n = Tree<N,A>::insert_(node);
	calc_upward(n);
	return n;
}

template<class N, class A>
typename tiq::tree::CountTree<N,A>::internal_node_ptr_t tiq::tree::CountTree<N,A>::erase_(internal_node_ptr_t node)
{
	internal_node_ptr_t n = Tree<N,A>::erase_(node);
	calc_upward(n);
	return n;
}

template<class N, class A>
void tiq::tree::CountTree<N,A>::left_rotate(internal_node_ptr_t x)
{
	Tree<N,A>::left_rotate(x);
	calc_count(x);
	calc_count(this->parent_(x));
}

template<class N, class A>
void tiq::tree::CountTree<N,A>::right_rotate(internal_node_ptr_t x)
{
	Tree<N,A>::right_rotate(x);
	calc_count(x);
	calc_count(this->parent_(x));
}

template<class N, class A>
void tiq::tree::CountTree<N,A>::transplant(internal_node_ptr_t u, internal_node_ptr_t v)
{
	Tree<N,A>::transplant(u, v);
	if (!v->is_end()) {
		calc_count(v);
	}
}

template<class N, class A>
void tiq::tree::CountTree<N,A>::calc_count(internal_node_ptr_t x) const
{
	if (x->is_end()) { return; }
	this->to_public_node(x)->count_ = this->to_public_node(this->left_(x))->count_ + this->to_public_node(this->right_(x))->count_ + 1;
}

template<class N, class A>
void tiq::tree::CountTree<N,A>::calc_upward(internal_node_ptr_t x) {
	do {
		calc_count(x);
	} while((x = this->parent_(x)));
}
