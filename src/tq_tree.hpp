#include <functional>
#include <stdexcept>
#include <memory>

template<class T>
Tiq::Tree<T>::Tree() : count_(0)
{
	root_ = begin_ = end_ = create_empty_node();
}

template<class T>
Tiq::Tree<T>::~Tree() {
	clear();
}

template<class T>
typename Tiq::Tree<T>::const_node_ptr_t Tiq::Tree<T>::root() const
{
	return root_;
}

template<class T>
typename Tiq::Tree<T>::const_node_ptr_t Tiq::Tree<T>::find(const_node_ptr_t cnode, comparator_fn_t comp) const
{
	auto node = const_cast<node_ptr_t>(cnode);
	while(true){
		if(node->is_end()){
			return node;
		}
		int comp_result = comp(node->get_data());
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

template<class T>
typename Tiq::Tree<T>::const_node_ptr_t Tiq::Tree<T>::find(comparator_fn_t comp) const
{
	return find(root_, comp);
}

template<class T>
typename Tiq::Tree<T>::const_node_ptr_t Tiq::Tree<T>::find_min(const_node_ptr_t cnode) const
{
	auto node = const_cast<node_ptr_t>(cnode);
	if(node->is_end()){
		return node;
	}
	while (!node->left_->is_end()) {
		node = node->left_;
	}
	return node;
}

template<class T>
typename Tiq::Tree<T>::const_node_ptr_t Tiq::Tree<T>::find_max(const_node_ptr_t cnode) const
{
	auto node = const_cast<node_ptr_t>(cnode);
	if(node->is_end()){
		return node;
	}
	while (!node->right_->is_end()) {
		node = node->right_;
	}
	return node;
}

template<class T>
typename Tiq::Tree<T>::const_node_ptr_t Tiq::Tree<T>::find_next(const_node_ptr_t cnode) const
{
	auto x = const_cast<node_ptr_t>(cnode);
	if (x->is_end()) {
		return root_ == x ? x : x->parent_;
	}
	if (!x->right_->is_end()) {
		return find_min(x->right_);
	}
	node_ptr_t y = x->parent_;
	while (y != nullptr && x == y->right_) {
		x = y;
		y = y->parent_;
	}
	if(!y){
		return end();
	}
	return y;
}

template<class T>
typename Tiq::Tree<T>::const_node_ptr_t Tiq::Tree<T>::find_prev(const_node_ptr_t cnode) const
{
	auto x = const_cast<node_ptr_t>(cnode);
	if (x->is_end()) {
		return root_ == x ? x : find_prev(x->parent_);
	}
	if (!x->left_->is_end()) {
		return find_max(x->left_);
	}
	node_ptr_t y = x->parent_;
	while (y != nullptr && x == y->left_) {
		x = y;
		y = y->parent;
	}
	if(!y){
		return end();
	}
	return y;
}

template<class T>
typename Tiq::Tree<T>::const_node_ptr_t Tiq::Tree<T>::insert(const_node_ptr_t cnode, T&& data)
{
	auto node = const_cast<node_ptr_t>(cnode);
	node->data_ = std::forward<T>(data);

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

template<class T>
typename Tiq::Tree<T>::const_node_ptr_t Tiq::Tree<T>::erase(const_node_ptr_t cnode) {
	auto z = const_cast<node_ptr_t>(cnode);
	if (z->is_end()) {
		throw std::logic_error("Cannot erase end node");
	}

	node_ptr_t x, y = z;
	bool y_original_color = y->color_;
	if (z->left_->is_end()) {
		delete_node(z->left_);
		x = z->right_;
		rb_transplant(z, x);
	} else if (z->right_->is_end()) {
		delete_node(z->right_);
		x = z->left_;
		rb_transplant(z, x);
	} else {
		y = find_min(z->right_);
		y_original_color = y->color_;
		x = y->right_;

		if (y->parent_ != z) {
			rb_transplant(y, y->right);
			y->right_ = z->right_;
			y->right_->parent_ = y;
		}

		rb_transplant(z, y);
		// Delete end node.
		delete_node(y->left_);
		y->left_ = z->left_;
		y->left_->parent_ = y;
		y->color_ = z->color_;
	}
	if (y_original_color == 0){
		fix_delete(x);
	}

	count_--;

	delete_node(z);

	return x;
}

template<class T>
size_t Tiq::Tree<T>::size() const
{
	return count_;
}

template<class T>
typename Tiq::Tree<T>::const_node_ptr_t Tiq::Tree<T>::begin() const
{
	return begin_ == root_ ? begin_ : begin_->parent_;
}

template<class T>
typename Tiq::Tree<T>::const_node_ptr_t Tiq::Tree<T>::end() const
{
	return end_;
}

/* PRIVATE */

// rotate left at node x
template<class T>
void Tiq::Tree<T>::left_rotate(node_ptr_t x) {
	node_ptr_t y = x->right_;
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
template<class T>
void Tiq::Tree<T>::right_rotate(node_ptr_t x) {
	node_ptr_t y = x->left_;
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
template<class T>
void Tiq::Tree<T>::fix_delete(node_ptr_t x) {
	node_ptr_t s;
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
				s->color = x->parent_->color_;
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


template<class T>
void Tiq::Tree<T>::rb_transplant(node_ptr_t u, node_ptr_t v){
	if (u->parent_ == nullptr) {
		root_ = v;
	} else if (u == u->parent_->left_){
		u->parent_->left_ = v;
	} else {
		u->parent_->right_ = v;
	}
	v->parent_ = u->parent_;
}

template<class T>
void Tiq::Tree<T>::dfs(node_ptr_t node, std::function<void(node_ptr_t)> fn)
{
	if(node->is_end()){
		return;
	}
	dfs(node->left_, fn);
	dfs(node->right_, fn);
	fn(node);
}

template<class T>
void Tiq::Tree<T>::clear()
{
	dfs(root_, [](node_ptr_t node){
		delete node;
	});
	root_ = begin_ = end_ = create_empty_node();
	count_ = 0;
}

// fix the red-black tree
template<class T>
void Tiq::Tree<T>::fix_insert(node_ptr_t k){
	node_ptr_t u;
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

template<class T>
typename Tiq::Tree<T>::node_ptr_t Tiq::Tree<T>::create_empty_node() {
	return new Node();
}

template<class T>
void Tiq::Tree<T>::delete_node(node_ptr_t node) {
	delete node;
}
