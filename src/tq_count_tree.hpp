#include "tq_tree.h"

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
