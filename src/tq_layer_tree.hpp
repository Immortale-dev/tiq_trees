template<class K, class A>
void Tiq::Tree::LayersCollection<K,A>::set(K key, size_t value)
{
	auto node = bs_find(key);
	if (!node->is_end()) {
		node->value_ = value;
		this->insert(node, key);
		return;
	}
	// Actual insert.
	node->value_ = value;
	this->insert(node, key);
}

template<class K, class A>
void Tiq::Tree::LayersCollection<K,A>::unset(K key)
{
	auto node = bs_find(key);
	if (!node->is_end()) {
		this->erase(node);
	}
}

template<class K, class A>
size_t Tiq::Tree::LayersCollection<K,A>::get(K key)
{
	auto node = bs_find(key);

	if (node->is_end()) return 0;
	return node->value_;
}

template<class K, class A>
size_t Tiq::Tree::LayersCollection<K,A>::count(K key)
{
	auto node = bs_find(key);

	// Get upper bound.
	node = this->find_next(node);

	// Move one back.
	node = this->find_prev(node);

	if (!node || node->is_end()) return 0;

	return this->left(node)->count() + node->value_;
}

template<class K, class A>
void Tiq::Tree::LayersCollection<K,A>::add(key_t key, long int value)
{
	auto node = bs_find(key);
	if (!node->is_end()) {
		node->value_ = node->value_ + value;
		this->insert(node, key);
		return;
	}
	// Insert if doesn't exist.
	node->value_ = value;
	this->insert(node, key);
}

template<class K, class A>
void Tiq::Tree::LayersCollection<K,A>::merge(const LayersCollection<K,A>& collection)
{
	auto min = collection->find_min();
	while (!min->is_end()) {
		this->add(min->data(), min->value_);
		min = collection->find_next(min);
	}
}

template<class K, class A>
Tiq::Tree::LayerStatNode<K>* Tiq::Tree::LayerCollection<K,A>::bs_find(K key)
{
	auto node = this->root();
	while(!node->is_end()) {
		if (key == node->data()) {
			// Item found.
			break;
		}
		if (key > node->data()) {
			node = this->right(node);
		} else {
			node = this->left(node);
		}
	}
	return node;
}

template<class K, class A>
void Tiq::Tree::LayerCollection<K,A>::calc_count(node_ptr_t x)
{
	if (x->is_end()) return;

	auto left = this->left(x);
	auto right = this->right(x);
	auto n = this->to_public_node(x);

	n->count_ = left->count() + right->count() + n->value_;
}

// LayerTree

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::insert(const_node_ptr_t node, T data, layer_key_t layer)
{
	if (node->is_end()) {
		throw std::logic_error("The node is not end node.");
	}
	node->layer_ = layer;
	node->data() = data;
	auto node = this->insert_(node);
	upward_layer_count_update(node, layer);
	return node;
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::update(const_node_ptr_t node, T data)
{
	if (!node->is_end()) {
		throw std::logic_error("Cannot update end node.");
	}
	node->data() = data;
	auto node = this->insert_(node);
	upward_layer_count_update(node, layer);
	return node;
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::erase(const_node_ptr_t node)
{
	auto node = this->erase_(node);
	upward_layer_count_update(node, layer);
	return node;
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find(comparator_fn_t comp) const {
	return CountTree<N,A>::find(comp);
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find(const_node_ptr_t node, comparator_fn_t comp) const {
	return CountTree<N,A>::find(node, comp);
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find_min(const_node_ptr_t node = nullptr) const {
	return CountTree<N,A>::find_min(node);
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find_max(const_node_ptr_t node = nullptr) const {
	return CountTree<N,A>::find_max(node);
]

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find_min(const_node_ptr_t node, layer_key_t layer) const {
	const_node_ptr_t n = node;
	const_node_ptr_t saved = nullptr;
	while(!n->is_end()) {
		if (n->layer() <= layer) {
			saved = n;
		}
		n = this->left(n);
	}
	if (saved) {
		return saved;
	}
	return n;
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find_max(const_node_ptr_t node, layer_key_t layer) const {
	const_node_ptr_t n = node;
	const_node_ptr_t saved = nullptr;
	while(!n->is_end()) {
		if (n->layer() <= layer) {
			saved = n;
		}
		n = this->right(n);
	}
	if (saved) {
		return saved;
	}
	return n;
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find_min(layer_key_t layer) const {
	return find_min(this->root(), layer);
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find_max(layer_key_t layer) const {
	return find_max(this->root(), layer);
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find_next(const_node_ptr_t node) const {
	return CountTree<N,A>::find_next(node);
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find_prev(const_node_ptr_t node) const {
	return CountTree<N,A>::find_prev(node);
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find_next(const_node_ptr_t node, layer_key_t layer) const {
	if (node->is_end()) {
		if (node == this->root()) {
			return node;
		}

		while(node) {
			const_node_ptr_t par = this->parent(node);
			if (!par) {
				return this->end();
			}
			if (node == this->right(par)) {
				node = par;
				continue;
			}
			if (node == this->left(par) && par->layer() == layer) {
				return par;
			}
			node = par;
			if (this->right(node)->count(layer)) {
				return find_nth(this->right(node), 0, layer);
			}
		}
	}
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find_prev(const_node_ptr_t node, layer_key_t layer) const;

template<class N, class A>
void Tiq::Tree::LayerTree<N,A>::left_rotate(node_ptr_t x)
{
	CountTree<N,A>::left_rotate(x);
	wide_count_update(x);
	wide_count_update(this->parent_(x));
}

template<class N, class A>
void Tiq::Tree::LayerTree<N,A>::right_rotate(node_ptr_t x)
{
	CountTree<N,A>::left_rotate(x);
	wide_count_update(x);
	wide_count_update(this->parent_(x));
}

template<class N, class A>
void Tiq::Tree::LayerTree<N,A>::transplant(node_ptr_t u, node_ptr_t v)
{
	CountTree<N,A>::transplant(u, v);
	if (this->parent_(v)) {
		wide_count_update(this->parent_(v));
	}
}

template<class N, class A>
void Tiq::Tree::LayerTree<N,A>::upward_layer_count_update(node_ptr_t node, layer_key_t layer)
{
	auto n = this->to_public_node(node);
	while(n && !n->is_end()) {
		layer_count_update(n, layer);
		n = this->parent(n);
	}
}

template<class N, class A>
void Tiq::Tree::LayerTree<N,A>::layer_count_update(node_ptr_t node, layer_key_t layer)
{
	auto n = this->to_public_node(node);
	auto left = this->to_public_node(this->left_(node));
	auto right = this->to_public_node(this->right_(node));

	size_t value = left->layers_.get(layer) + right->layers_.get(layer);
	if (node->layer_ == layer) {
		++value;
	}
	n->layers_.set(value);
}

template<class N, class A>
void Tiq::Tree::LayerTree<N,A>::wide_count_update(node_ptr_t node)
{
	auto n = this->to_public_node(node);
	auto& layers = n->layers_;
	auto left = this->to_public_node(this->left_(node));
	auto right = this->to_public_node(this->right_(node));

	layers.clear();
	layers.merge(left->layers_);
	layers.merge(right->layers_);
	layers.add(n->layer(), 1);
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find_nth(const_node_ptr_t node, size_t count) const
{
	return CountTree<N,A>::find_nth(node, count);
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find_nth(size_t count) const
{
	return CountTree<N,A>::find_nth(count);
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find_nth(const_node_ptr_t node, size_t count, layer_key_t layer) const
{
	if (node->is_end()) {
		return node;
	}
	if (node->count(layer) <= count) {
		return this->right(this->find_max(node));
	}
	while(true) {
		size_t left_count = this->left(node)->count(layer);
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
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find_nth(size_t count, layer_ket_t layer) const
{
	return find_nth(this->root(), count, layer);
}

template<class N, class A>
size_t Tiq::Tree::LayerTree<N,A>::find_index(const_node_ptr_t node const_node_ptr_t parent) const
{
	return CountTree<N,A>::find_index(node, parent);
}

template<class N, class A>
size_t Tiq::Tree::LayerTree<N,A>::find_index(const_node_ptr_t node, const_node_ptr_t parent, layer_key_t layer) const
{
	size_t count = 0;
	if (!node->is_end()) {
		count += this->left(node)->count(layer);
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
			count += this->left(par)->count(layer);
			if (par->layer() <= layer) {
				++count;
			}
		}
		node = par;
	}
	return count;
}

template<class N, class A>
size_t Tiq::Tree::LayerTree<N,A>::find_index(const_node_ptr_t node, layer_key_t layer) const
{
	return find_index(node, this->root(), layer);
}
