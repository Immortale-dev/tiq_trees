#include <iostream> // TODO: remove

#include <stdexcept>

#include "tq_tree.h"

template<class N, class A>
typename Tiq::Tree::CountTree<N,A>::const_node_ptr_t Tiq::Tree::CountTree<N,A>::find_nth(const_node_ptr_t node, size_t count) const
{
	if (node->is_end()) {
		return node;
	}
	if (node->count() <= count) {
		return this->right(this->find_max(node));
	}
	while(true) {
		size_t left_count = this->left(node)->count();
		if (left_count == count) {
			return node;
		}
		if (left_count < count) {
			count -= left_count + 1; // + current node.
			node = this->right(node);
		} else {
			node = this->left(node);
		}
	}
}

template<class N, class A>
typename Tiq::Tree::CountTree<N,A>::const_node_ptr_t Tiq::Tree::CountTree<N,A>::find_nth(size_t count) const
{
	return find_nth(this->root(), count);
}

template<class N, class A>
size_t Tiq::Tree::CountTree<N,A>::find_index(const_node_ptr_t node, const_node_ptr_t parent) const
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
typename Tiq::Tree::CountTree<N,A>::node_ptr_t Tiq::Tree::CountTree<N,A>::insert_(node_ptr_t node)
{
	node_ptr_t n = Tree<N,A>::insert_(node);
	calc_upward(n);
	return n;
}

template<class N, class A>
typename Tiq::Tree::CountTree<N,A>::node_ptr_t Tiq::Tree::CountTree<N,A>::erase_(node_ptr_t node)
{
	node_ptr_t n = Tree<N,A>::erase_(node);
	calc_upward(n);
	return n;
}

template<class N, class A>
void Tiq::Tree::CountTree<N,A>::left_rotate(node_ptr_t x)
{
	Tree<N,A>::left_rotate(x);
	calc_count(x);
	calc_count(this->parent_(x));
}

template<class N, class A>
void Tiq::Tree::CountTree<N,A>::right_rotate(node_ptr_t x)
{
	Tree<N,A>::right_rotate(x);
	calc_count(x);
	calc_count(this->parent_(x));
}

template<class N, class A>
void Tiq::Tree::CountTree<N,A>::transplant(node_ptr_t u, node_ptr_t v)
{
	Tree<N,A>::transplant(u, v);
	if (this->parent_(v)) {
		calc_count(this->parent_(v));
	}
}

template<class N, class A>
void Tiq::Tree::CountTree<N,A>::calc_count(node_ptr_t x)
{
	if (x->is_end()) {
		this->to_public_node(x)->count_ = 0;
		return;
	}
	this->to_public_node(x)->count_ = this->to_public_node(this->left_(x))->count_ + this->to_public_node(this->right_(x))->count_ + 1;
}

template<class N, class A>
void Tiq::Tree::CountTree<N,A>::calc_upward(node_ptr_t x) {
	do {
		calc_count(x);
	} while((x = this->parent_(x)));
}
