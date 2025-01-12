#include <stdexcept>

template<class N, class A>
typename tiq::tree::LeafTree<N,A>::node_ptr_t tiq::tree::LeafTree<N,A>::find_next_leaf(node_ptr_t node) const
{
	do {
		node = this->find_next(node);
	} while(!node->is_end() && !node->is_leaf());
	return node;
}

template<class N, class A>
typename tiq::tree::LeafTree<N,A>::node_ptr_t tiq::tree::LeafTree<N,A>::find_prev_leaf(node_ptr_t node) const
{
	do {
		node = this->find_prev(node);
	} while(!node->is_end() && !node->is_leaf());
	return node;
}

template<class N, class A>
typename tiq::tree::LeafTree<N,A>::internal_node_ptr_t tiq::tree::LeafTree<N,A>::insert_(internal_node_ptr_t node)
{
	// If the node is not end, just return as the structure wasn't currupted.
	if (!node->is_end()) {
		return node;
	}
	if (node == this->root_) {
		return Tree<N,A>::insert_(node);
	}

	node_ptr_t parent = this->to_public_node(this->parent_(node));
	bool parent_color = parent->color_;

	node_ptr_t internal_node = this->to_public_node(this->create_empty_node());
	internal_node->color_ = parent_color;

	internal_node->left_ = node;
	internal_node->right_ = parent;
	if (parent->right_ == node) {
		std::swap(internal_node->left_, internal_node->right_);
	}
	node_ptr_t g_parent = this->to_public_node(parent->parent_);
	if (g_parent) {
		if (g_parent->left_ == parent) {
			g_parent->left_ = internal_node;
		} else {
			g_parent->right_ = internal_node;
		}
	} else {
		this->root_ = internal_node;
	}
	internal_node->parent_ = g_parent;

	if (parent->left_ == node) {
		parent->left_ = this->create_empty_node();
	} else {
		parent->right_ = this->create_empty_node();
	}
	this->to_public_node(node)->parent_ = internal_node;
	parent->parent_ = internal_node;

	parent->color_ = 1;

	this->to_public_node(node)->leaf_ = true;
	return Tree<N,A>::insert_(node);
}

template<class N, class A>
typename tiq::tree::LeafTree<N,A>::internal_node_ptr_t tiq::tree::LeafTree<N,A>::erase_(internal_node_ptr_t node)
{
	node_ptr_t z = this->to_public_node(node);
	if (z->is_end()) {
		throw std::logic_error("Cannot erase end node");
	}
	if (!this->to_public_node(z)->is_leaf()) {
		throw std::logic_error("Cannot erase internal node");
	}

	if (z == this->root_) {
		return Tree<N,A>::erase_(node);
	}

	node_ptr_t parent = this->to_public_node(z->parent_);
	node_ptr_t other = parent->left_ == z ? this->right(parent) : this->left(parent);
	if (z->left_ == this->begin_) {
		// Reassign begin_ node.
		this->begin_ = other;
		while(!this->begin_->is_end()){
			this->begin_ = this->to_public_node(this->begin_)->left_;
		}
	}
	if (z->right_ == this->end_) {
		// Reassign end_ node.
		this->end_ = other;
		while(!this->end_->is_end()) {
			this->end_ = this->to_public_node(this->end_)->right_;
		}
	}
	this->delete_node(z->left_);
	this->delete_node(z->right_);

	this->transplant(parent, other);
	other->color_ = 0;

	if (parent->color_ == 0 && (z->color_ == 0 || other->color_ == 0)) {
		this->fix_delete(other);
	}

	this->count_--;

	this->delete_node(z);
	this->delete_node(parent);
	return other;
}
