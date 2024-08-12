#include <functional>
#include <stdexcept>
#include <memory>

template<class N, class A>
Tiq::Tree::Tree<N,A>::Tree() : count_(0)
{
	root_ = begin_ = end_ = create_empty_node();
}

template<class N, class A>
Tiq::Tree::Tree<N,A>::~Tree() {
	dfs(root_, [this](internal_node_ptr_t node){
		this->delete_node(node);
	});
}

template<class N, class A>
typename Tiq::Tree::Tree<N,A>::node_ptr_t Tiq::Tree::Tree<N,A>::root() const
{
	return to_public_node(root_);
}

template<class N, class A>
typename Tiq::Tree::Tree<N,A>::internal_node_ptr_t Tiq::Tree::Tree<N,A>::find_(internal_node_ptr_t node, comparator_fn_t comp) const
{
	while(true){
		if(node->is_end()){
			return node;
		}
		int comp_result = comp(static_cast<node_ptr_t>(node)->data());
		if (comp_result == 0) {
			return node;
		}
		if(comp_result > 0) {
			node = node->right_;
		} else {
			node = node->left_;
		}
	}
	return nullptr;
}

template<class N, class A>
typename Tiq::Tree::Tree<N,A>::node_ptr_t Tiq::Tree::Tree<N,A>::find(node_ptr_t cnode, comparator_fn_t comp) const
{
	return to_public_node(find_(to_internal_node(cnode), comp));
}

template<class N, class A>
typename Tiq::Tree::Tree<N,A>::node_ptr_t Tiq::Tree::Tree<N,A>::find(comparator_fn_t comp) const
{
	return to_public_node(find_(root_, comp));
}

template<class N, class A>
typename Tiq::Tree::Tree<N,A>::internal_node_ptr_t Tiq::Tree::Tree<N,A>::find_min_(internal_node_ptr_t node) const
{
	if(node->is_end()){
		return node;
	}
	while (!node->left_->is_end()) {
		node = node->left_;
	}
	return node;
}

template<class N, class A>
typename Tiq::Tree::Tree<N,A>::node_ptr_t Tiq::Tree::Tree<N,A>::find_min(node_ptr_t cnode) const
{
	internal_node_ptr_t node = cnode ? to_internal_node(cnode) : root_;
	return to_public_node(find_min_(node));
}

template<class N, class A>
typename Tiq::Tree::Tree<N,A>::node_ptr_t Tiq::Tree::Tree<N,A>::find_max(node_ptr_t cnode) const
{
	internal_node_ptr_t node = cnode ? to_internal_node(cnode) : root_;
	return to_public_node(find_max_(node));
}

template<class N, class A>
typename Tiq::Tree::Tree<N,A>::internal_node_ptr_t Tiq::Tree::Tree<N,A>::find_max_(internal_node_ptr_t node) const
{
	if(node->is_end()){
		return node;
	}
	while (!node->right_->is_end()) {
		node = node->right_;
	}
	return node;
}

template<class N, class A>
typename Tiq::Tree::Tree<N,A>::node_ptr_t Tiq::Tree::Tree<N,A>::find_next(node_ptr_t cnode) const
{
	auto x = to_internal_node(cnode);
	if (x->is_end()) {
		if (x == root_) {
			return to_public_node(x);
		}
		if (x == x->parent_->left_) {
			return to_public_node(x->parent_);
		}
		x = x->parent_;
	}
	if (!x->right_->is_end()) {
		return to_public_node(find_min_(x->right_));
	}
	internal_node_ptr_t y = x->parent_;
	while (y != nullptr && x == y->right_) {
		x = y;
		y = y->parent_;
	}
	if(!y){
		return to_public_node(end_);
	}
	return to_public_node(y);
}

template<class N, class A>
typename Tiq::Tree::Tree<N,A>::node_ptr_t Tiq::Tree::Tree<N,A>::find_prev(node_ptr_t cnode) const
{
	auto x = to_internal_node(cnode);
	if (x->is_end()) {
		if (x == root_) {
			return to_public_node(x);
		}
		if (x == x->parent_->right_) {
			return to_public_node(x->parent_);
		}
		x = x->parent_;
	}
	if (!x->left_->is_end()) {
		return to_public_node(find_max_(x->left_));
	}
	internal_node_ptr_t y = x->parent_;
	while (y != nullptr && x == y->left_) {
		x = y;
		y = y->parent_;
	}
	if(!y){
		return to_public_node(begin_);
	}
	return to_public_node(y);
}

template<class N, class A>
typename Tiq::Tree::Tree<N,A>::node_ptr_t Tiq::Tree::Tree<N,A>::parent(node_ptr_t cnode) const
{
	return to_public_node(cnode->parent_);
}

template<class N, class A>
typename Tiq::Tree::Tree<N,A>::node_ptr_t Tiq::Tree::Tree<N,A>::left(node_ptr_t cnode) const
{
	return to_public_node(cnode->left_);
}

template<class N, class A>
typename Tiq::Tree::Tree<N,A>::node_ptr_t Tiq::Tree::Tree<N,A>::right(node_ptr_t cnode) const
{
	return to_public_node(cnode->right_);
}

template<class N, class A>
typename Tiq::Tree::Tree<N,A>::node_ptr_t Tiq::Tree::Tree<N,A>::before(node_ptr_t cnode) const
{
	if (!cnode) {
		return to_public_node(begin_);
	}
	if (cnode->is_end()) {
		return cnode;
	}
	internal_node_ptr_t node = find_max_(left_(to_internal_node(cnode)));
	if (!node->is_end()) {
		node = right_(node);
	}
	return to_public_node(node);
}

template<class N, class A>
typename Tiq::Tree::Tree<N,A>::node_ptr_t Tiq::Tree::Tree<N,A>::after(node_ptr_t cnode) const
{
	if (!cnode) {
		return end();
	}
	if (cnode->is_end()) {
		return cnode;
	}
	internal_node_ptr_t node = find_min_(right_(to_internal_node(cnode)));
	if (!node->is_end()) {
		node = left_(node);
	}
	return to_public_node(node);
}

template<class N, class A>
typename Tiq::Tree::Tree<N,A>::node_ptr_t Tiq::Tree::Tree<N,A>::insert(node_ptr_t cnode, T data)
{
	cnode->data_ = data;
	internal_node_ptr_t node = to_internal_node(cnode);
	return to_public_node(insert_(node));
}

template<class N, class A>
typename Tiq::Tree::Tree<N,A>::internal_node_ptr_t Tiq::Tree::Tree<N,A>::insert_(internal_node_ptr_t node)
{
	// If the node is not end, just return as the structure wasn't currupted.
	if (!node->is_end()) {
		return node;
	}

	node->is_end_ = false;
	node->left_ = create_empty_node();
	node->right_ = create_empty_node();
	node->color_ = 1;

	node->left_->parent_ = node;
	node->right_->parent_ = node;

	if (begin_ == node) {
		begin_ = node->left_;
	}
	if (end_ == node) {
		end_ = node->right_;
	}

	count_++;

	// if new node is a root node, simply return
	if (node->parent_ == nullptr){
		node->color_ = 0;
		return node;
	}

	// if the grandparent is null, simply return
	if (node->parent_->parent_ == nullptr) {
		return node;
	}

	// Fix the tree
	fix_insert(node);
	return node;
}

template<class N, class A>
typename Tiq::Tree::Tree<N,A>::node_ptr_t Tiq::Tree::Tree<N,A>::erase(node_ptr_t cnode)
{
	return to_public_node(erase_(to_internal_node(cnode)));
}

template<class N, class A>
typename Tiq::Tree::Tree<N,A>::internal_node_ptr_t Tiq::Tree::Tree<N,A>::erase_(internal_node_ptr_t node)
{
	internal_node_ptr_t z = node;
	if (z->is_end()) {
		throw std::logic_error("Cannot erase end node");
	}

	internal_node_ptr_t x, y = z;
	bool y_original_color = y->color_;
	if (z->left_->is_end()) {
		if (z->left_ == begin_) {
			// Reassign begin_ node.
			begin_ = z->right_;
			while(!begin_->is_end()){
				begin_ = begin_->left_;
			}
		}
		delete_node(z->left_);
		x = z->right_;
		transplant(z, x);
	} else if (z->right_->is_end()) {
		if (z->right_ == end_) {
			// Reassign end_ node.
			end_ = z->left_;
			while(!end_->is_end()) {
				end_ = end_->right_;
			}
		}
		delete_node(z->right_);
		x = z->left_;
		transplant(z, x);
	} else {
		y = find_min_(z->right_);
		y_original_color = y->color_;
		x = y->right_;

		if (y->parent_ != z) {
			transplant(y, y->right_);
			y->right_ = z->right_;
			y->right_->parent_ = y;
		}

		// Delete end node.
		delete_node(y->left_);
		y->left_ = z->left_;
		y->left_->parent_ = y;
		y->color_ = z->color_;
		transplant(z, y);
	}
	if (y_original_color == 0){
		fix_delete(x);
	}

	count_--;

	delete_node(z);

	return x;
}

template<class N, class A>
size_t Tiq::Tree::Tree<N,A>::size() const
{
	return count_;
}

template<class N, class A>
typename Tiq::Tree::Tree<N,A>::node_ptr_t Tiq::Tree::Tree<N,A>::begin() const
{
	auto node = begin_ == root_ ? begin_ : begin_->parent_;
	return to_public_node(node);
}

template<class N, class A>
typename Tiq::Tree::Tree<N,A>::node_ptr_t Tiq::Tree::Tree<N,A>::end() const
{
	return to_public_node(end_);
}

/* PRIVATE */

// rotate left at node x
template<class N, class A>
void Tiq::Tree::Tree<N,A>::left_rotate(internal_node_ptr_t x)
{
	internal_node_ptr_t y = x->right_;
	x->right_ = y->left_;
	y->left_->parent_ = x;
	y->parent_ = x->parent_;
	if (x->parent_ == nullptr) {
		this->root_ = y;
	} else if (x == x->parent_->left_) {
		x->parent_->left_ = y;
	} else {
		x->parent_->right_ = y;
	}
	y->left_ = x;
	x->parent_ = y;
}

// rotate right at node x
template<class N, class A>
void Tiq::Tree::Tree<N,A>::right_rotate(internal_node_ptr_t x)
{
	internal_node_ptr_t y = x->left_;
	x->left_ = y->right_;
	y->right_->parent_ = x;
	y->parent_ = x->parent_;
	if (x->parent_ == nullptr) {
		this->root_ = y;
	} else if (x == x->parent_->right_) {
		x->parent_->right_ = y;
	} else {
		x->parent_->left_ = y;
	}
	y->right_ = x;
	x->parent_ = y;
}


// fix the rb tree modified by the delete operation
template<class N, class A>
void Tiq::Tree::Tree<N,A>::fix_delete(internal_node_ptr_t x)
{
	internal_node_ptr_t s;
	while (x != root_ && x->color_ == 0) {
		if (x == x->parent_->left_) {
			s = x->parent_->right_;
			if (s->color_ == 1) {
				// case 3.1
				s->color_ = 0;
				x->parent_->color_ = 1;
				left_rotate(x->parent_);
				s = x->parent_->right_;
			}

			if (s->left_->color_ == 0 && s->right_->color_ == 0) {
				// case 3.2
				s->color_ = 1;
				x = x->parent_;
			} else {
				if (s->right_->color_ == 0) {
					// case 3.3
					s->left_->color_ = 0;
					s->color_ = 1;
					right_rotate(s);
					s = x->parent_->right_;
				}
				// case 3.4
				s->color_ = x->parent_->color_;
				x->parent_->color_ = 0;
				s->right_->color_ = 0;
				left_rotate(x->parent_);
				x = root_;
			}
		} else {
			s = x->parent_->left_;
			if (s->color_ == 1) {
				// case 3.1
				s->color_ = 0;
				x->parent_->color_ = 1;
				right_rotate(x->parent_);
				s = x->parent_->left_;
			}
			if (s->left_->color_ == 0 && s->right_->color_ == 0) {
				// case 3.2
				s->color_ = 1;
				x = x->parent_;
			} else {
				if (s->left_->color_ == 0) {
					// case 3.3
					s->right_->color_ = 0;
					s->color_ = 1;
					left_rotate(s);
					s = x->parent_->left_;
				}
				// case 3.4
				s->color_ = x->parent_->color_;
				x->parent_->color_ = 0;
				s->left_->color_ = 0;
				right_rotate(x->parent_);
				x = root_;
			}
		}
	}
	x->color_ = 0;
}

template<class N, class A>
void Tiq::Tree::Tree<N,A>::transplant(internal_node_ptr_t u, internal_node_ptr_t v)
{
	if (u->parent_ == nullptr) {
		root_ = v;
	} else if (u == u->parent_->left_){
		u->parent_->left_ = v;
	} else {
		u->parent_->right_ = v;
	}
	v->parent_ = u->parent_;
}

template<class N, class A>
typename Tiq::Tree::Tree<N,A>::internal_node_ptr_t Tiq::Tree::Tree<N,A>::left_(internal_node_ptr_t x) const
{
	return x->left_;
}

template<class N, class A>
typename Tiq::Tree::Tree<N,A>::internal_node_ptr_t Tiq::Tree::Tree<N,A>::right_(internal_node_ptr_t x) const
{
	return x->right_;
}

template<class N, class A>
typename Tiq::Tree::Tree<N,A>::internal_node_ptr_t Tiq::Tree::Tree<N,A>::parent_(internal_node_ptr_t x) const
{
	return x->parent_;
}

template<class N, class A>
typename Tiq::Tree::Tree<N,A>::node_ptr_t Tiq::Tree::Tree<N,A>::to_public_node(internal_node_ptr_t node) const
{
	return const_cast<node_ptr_t>(static_cast<N*>(node));
}

template<class N, class A>
typename Tiq::Tree::Tree<N,A>::internal_node_ptr_t Tiq::Tree::Tree<N,A>::to_internal_node(node_ptr_t node) const
{
	return const_cast<internal_node_ptr_t>(static_cast<typename std::add_const<internal_node_ptr_t>::type>(node));
}

template<class N, class A>
void Tiq::Tree::Tree<N,A>::dfs(internal_node_ptr_t node, std::function<void(internal_node_ptr_t)> fn)
{
	if(!node->is_end()){
		dfs(node->left_, fn);
		dfs(node->right_, fn);
	}
	fn(node);
}

template<class N, class A>
void Tiq::Tree::Tree<N,A>::clear()
{
	dfs(root_, [this](internal_node_ptr_t node){
		this->delete_node(node);
	});
	root_ = begin_ = end_ = create_empty_node();
	count_ = 0;
}

// fix the red-black tree
template<class N, class A>
void Tiq::Tree::Tree<N,A>::fix_insert(internal_node_ptr_t k)
{
	internal_node_ptr_t u;
	while (k->parent_->color_ == 1) {
		if (k->parent_ == k->parent_->parent_->right_) {
			u = k->parent_->parent_->left_; // uncle
			if (u->color_ == 1) {
				// case 3.1
				u->color_ = 0;
				k->parent_->color_ = 0;
				k->parent_->parent_->color_ = 1;
				k = k->parent_->parent_;
			} else {
				if (k == k->parent_->left_) {
					// case 3.2.2
					k = k->parent_;
					right_rotate(k);
				}
				// case 3.2.1
				k->parent_->color_ = 0;
				k->parent_->parent_->color_ = 1;
				left_rotate(k->parent_->parent_);
			}
		} else {
			u = k->parent_->parent_->right_; // uncle

			if (u->color_ == 1) {
				// mirror case 3.1
				u->color_ = 0;
				k->parent_->color_ = 0;
				k->parent_->parent_->color_ = 1;
				k = k->parent_->parent_;
			} else {
				if (k == k->parent_->right_) {
					// mirror case 3.2.2
					k = k->parent_;
					left_rotate(k);
				}
				// mirror case 3.2.1
				k->parent_->color_ = 0;
				k->parent_->parent_->color_ = 1;
				right_rotate(k->parent_->parent_);
			}
		}
		if (k == root_) {
			break;
		}
	}
	root_->color_ = 0;
}

template<class N, class A>
typename Tiq::Tree::Tree<N,A>::internal_node_ptr_t Tiq::Tree::Tree<N,A>::create_empty_node()
{
	node_ptr_t node = std::allocator_traits<A>::allocate(alloc_, 1);
	std::allocator_traits<A>::construct(alloc_, node);
	return node;
}

template<class N, class A>
void Tiq::Tree::Tree<N,A>::delete_node(internal_node_ptr_t node)
{
	std::allocator_traits<A>::destroy(alloc_, node);
	std::allocator_traits<A>::deallocate(alloc_, static_cast<N*>(node), 1);
}
