template<class N, class A>
tiq::tree::LeafTree<N,A>::node_ptr_t tiq::tree::LeafTree<N,A>::find_next_leaf(node_ptr_t node) const
{
	do {
		node = this->find_next(node);
	} while(!node->is_end() && !node->is_leaf());
	return node;
}

template<class N, class A>
tiq::tree::LeafTree<N,A>::node_ptr_t tiq::tree::LeafTree<N,A>::find_prev_leaf(node_ptr_t node) const
{
	do {
		node = this->find_prev(node);
	} while(!node->is_end() && !node->is_leaf());
	return node;
}

template<class N, class A>
tiq::tree::LeafTree<N,A>::internal_node_ptr_t tiq::tree::LeafTree<N,A>::insert_(internal_node_ptr_t node)
{
	// If the node is not end, just return as the structure wasn't currupted.
	if (!node->is_end()) {
		return node;
	}

	internal_node_ptr_t parent = this->parent_(node);
	bool parent_color = parent->color_;

	auto internal_node = this->create_empty_node();
	internal_node->color_ = node_color;

	internal_node->left_ = node;
	internal_node->right_ = parent;
	if (parent->right_ == node) {
		std::swap(internal_node->left_, internal_node->right_);
	}
	internal_node_ptr_t g_parent = parent->parent_;
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
	node->parent_ = internal_node;
	parent->parent_ = internal_node;

	parent->color_ = 1;

	node->leaf_ = true;
	return Tree<N,A>::insert_(node);
}

template<class N, class A>
tiq::tree::LeafTree<N,A>::internal_node_ptr_t tiq::tree::LeafTree<N,A>::erase_(internal_node_ptr_t node)
{
	internal_node_ptr_t z = node;
	if (z->is_end()) {
		throw std::logic_error("Cannot erase end node");
	}
	if (!this->to_public_node(z)->is_leaf()) {
		throw std::ligic_error("Cannot erase internal node");
	}

	internal_node_ptr_t x, y = z;
	bool y_original_color = y->color_;
	if (z == this->root_) {
		return Tree<N,A>::erase_(node);
	}

	internal_node_ptr_t parent = z->parent_;
	internal_node_ptr_t other = parent->left_ == z ? parent->right_ : parent->left_;
	if (z->left_ == begin_) {
		// Reassign begin_ node.
		begin_ = other;
		while(!begin_->is_end()){
			begin_ = begin_->left_;
		}
	}
	if (z->right_ == end_) {
		// Reassign end_ node.
		end_ = other;
		while(!end_->is_end()) {
			end_ = end_->right_;
		}
	}
	delete_node(z->left_);
	delete_node(z->right_);

	transplant(parent, other);
	other->color_ = 0;

	if (parent->color_ == 0 && (z->color_ == 0 || other->color_ == 0)) {
		fix_delete(other);
	}

	count_--;

	delete_node(z);
	delete_node(parent);
	return other;
}
