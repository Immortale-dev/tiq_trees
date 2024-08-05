// LayersCollection

template<class K, class A>
void Tiq::Tree::LayersCollection<K,A>::set(K key, value_t value)
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
typename Tiq::Tree::LayersCollection<K,A>::value_t Tiq::Tree::LayersCollection<K,A>::get(K key)
{
	auto node = bs_find(key);

	if (node->is_end()) return 0;
	return node->value_;
}

template<class K, class A>
typename Tiq::Tree::LayersCollection<K,A>::value_t Tiq::Tree::LayersCollection<K,A>::count(K key)
{
	auto node = bs_find(key);

	// Get upper bound.
	node = this->find_next(node);

	// Move one back.
	node = this->find_prev(node);

	if (!node || node->is_end()) return 0;

	value_t value = this->left(node)->count() + node->value_;

	while(node) {
		auto par = this->parent(node);
		if (par && this->right(par) == node) {
			value += this->left(par)->count() + par->value_;
		}
		node = par;
	}

	return value;
}

template<class K, class A>
void Tiq::Tree::LayersCollection<K,A>::add(key_t key, value_t value)
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
	auto min = collection.find_min();
	while (!min->is_end()) {
		this->add(min->data(), min->value_);
		min = collection.find_next(min);
	}
}

template<class K, class A>
Tiq::Tree::LayerStatNode<K>* Tiq::Tree::LayersCollection<K,A>::bs_find(K key)
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
void Tiq::Tree::LayersCollection<K,A>::calc_count(node_ptr_t x)
{
	CountTree<LayerStatNode<K>,A>::calc_count(x);

	if (x->is_end()) return;

	auto node = this->to_public_node(x);

	auto left = this->left(node);
	auto right = this->right(node);

	node->count_ = left->count() + right->count() + node->value_;
}

// ValuesCollection

template<class K, class T>
void Tiq::Tree::ValuesCollection<K,T>::set(K key, T value)
{
	auto node = bs_find(key);
	node->key_ = key;
	this->insert(node, value);
}

template<class K, class T>
void Tiq::Tree::ValuesCollection<K,T>::unset(K key)
{
	auto node = bs_find(key);
	if (!node->is_end()) {
		this->erase(node);
	}
}

template<class K, class T>
T& Tiq::Tree::ValuesCollection<K,T>::get(K key)
{
	auto node = bs_find(key);

	// Get upper bound.
	node = this->find_next(node);

	// Move one back.
	node = this->find_prev(node);

	if (!node || node->is_end()) {
		throw std::logic_error("No data found");
	}

	return node->data();
}

template<class K, class T>
T& Tiq::Tree::ValuesCollection<K,T>::get()
{
	auto node = this->parent(this->end());

	if (!node || node->is_end()) {
		throw std::logic_error("No data found");
	}

	return node->data();
}

template<class K, class T>
K Tiq::Tree::ValuesCollection<K,T>::min()
{
	if (!this->size()) {
		throw std::logic_error("No data found");
	}

	return this->begin()->key();
}

template<class K, class T>
bool Tiq::Tree::ValuesCollection<K,T>::is_min(K key)
{
	auto node = this->begin();
	return !node->is_end() && node->key() == key;
}

template<class K, class T>
bool Tiq::Tree::ValuesCollection<K,T>::has(K key)
{
	auto node = bs_find(key);

	// Get upper bound.
	node = this->find_next(node);

	// Move one back.
	node = this->find_prev(node);

	return (node && !node->is_end());
}

template<class K, class T>
bool Tiq::Tree::ValuesCollection<K,T>::has()
{
	auto node = this->parent(this->end());

	return (node && !node->is_end());
}


template<class K, class T>
bool Tiq::Tree::ValuesCollection<K,T>::contains(K key)
{
	auto node = bs_find(key);

	return (node && !node->is_end());
}

template<class K, class T>
std::vector<K> Tiq::Tree::ValuesCollection<K,T>::keys()
{
	std::vector<K> ret;
	auto b = this->begin();
	while(!b->is_end()) {
		ret.push_back(b->key());
		b = this->find_next(b);
	}
	return ret;
}

template<class K, class T>
std::vector<K> Tiq::Tree::ValuesCollection<K,T>::cut(K key)
{
	auto node = bs_find(key);
	if (!node) {
		node = this->find_next(node);
	}
	std::vector<ValueStatNode<K,T>*> delete_nodes;
	std::vector<K> ret;
	while(!node->is_end()) {
		delete_nodes.push_back(node);
		ret.push_back(node->key());
		node = this->find_next(node);
	}
	for (auto n : delete_nodes) {
		this->erase(n);
	}
	return ret;
}

template<class K, class T>
Tiq::Tree::ValueStatNode<K,T>* Tiq::Tree::ValuesCollection<K,T>::bs_find(K key)
{
	auto node = this->root();
	while(!node->is_end()) {
		if (key == node->key()) {
			// Item found.
			break;
		}
		if (key > node->key()) {
			node = this->right(node);
		} else {
			node = this->left(node);
		}
	}
	return node;
}

// LayerTree

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::insert(const_node_ptr_t node, T data, layer_key_t layer)
{
	if (!node->is_end()){
		if (node->is_cut_at(layer)) {
			node->is_cut_ = false;
		}
		if (node->is_cut(layer)) return nullptr;
	}
	layer_key_t min;
	bool change_min = false;
	if (!node->empty() && !node->is_layer(layer)) {
		min = node->min_key();
		change_min = true;
	}
	node->values_.set(layer, data);
	auto n = this->insert_(node);
	if (change_min) {
		upward_layer_count_update(n, min);
	}
	upward_layer_count_update(n, layer);
	return this->to_public_node(n);
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::erase(const_node_ptr_t node, layer_key_t layer, bool remove)
{
	if (node->is_end()) {
		throw std::logic_error("Cannot erase end node");
	}

	if (remove) {
		bool is_min = node->is_min(layer);
		node->values_.unset(layer);
		if (node->is_cut_at(layer)) {
			node->is_cut_ = false;
		}
		upward_layer_count_update(node, layer);
		if (is_min && !node->empty()) {
			upward_layer_count_update(node, node->min_key());
		}
	} else {
		std::vector<layer_key_t> removed = node->values_.cut(layer);
		for (auto &k : removed) {
			upward_layer_count_update(node, k);
		}
		if (!node->empty()) {
			node->is_cut_ = true;
			node->kut_key_ = layer;
			upward_layer_count_update(node, layer);
		}
	}

	if (!node->empty()) {
		return node;
	}
	auto n = this->erase_(node);
	return this->to_public_node(n);
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::erase(const_node_ptr_t node)
{
	if (node->is_end()) {
		throw std::logic_error("Cannot erase end node");
	}
	std::vector<layer_key_t> keys = node->values_.keys();
	node->values_.clear();
	for(auto& k : keys) {
		upward_layer_count_update(node, k);
	}
	auto n = this->erase_(node);
	return this->to_public_node(n);
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find(comparator_fn_t comp) const
{
	return CountTree<N,A>::find(comp);
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find(const_node_ptr_t node, comparator_fn_t comp) const
{
	return CountTree<N,A>::find(node, comp);
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find_min(const_node_ptr_t node) const
{
	return CountTree<N,A>::find_min(node);
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find_max(const_node_ptr_t node) const
{
	return CountTree<N,A>::find_max(node);
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find_min(const_node_ptr_t node, layer_key_t layer) const
{
	return this->find_nth(node, 0, layer);
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find_max(const_node_ptr_t node, layer_key_t layer) const
{
	size_t size = node->count(layer);
	return this->find_nth(node, size-1, layer);
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find_min(layer_key_t layer) const
{
	return find_min(this->root(), layer);
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find_max(layer_key_t layer) const
{
	return find_max(this->root(), layer);
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find_next(const_node_ptr_t node) const
{
	return CountTree<N,A>::find_next(node);
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find_prev(const_node_ptr_t node) const
{
	return CountTree<N,A>::find_prev(node);
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find_next(const_node_ptr_t node, layer_key_t layer) const
{
	if (node->is_end()) {
		if (node == this->root()) {
			return node;
		}
	}
	if (!node->is_end() && this->right(node)->count(layer)) {
		return find_nth(this->right(node), 0, layer);
	}
	while(true) {
		const_node_ptr_t par = this->parent(node);
		if (!par) {
			return this->end();
		}
		if (node == this->right(par)) {
			node = par;
			continue;
		}
		if (node == this->left(par) && par->is_layer(layer)) {
			return par;
		}
		node = par;
		if (this->right(node)->count(layer)) {
			return find_nth(this->right(node), 0, layer);
		}
	}
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find_prev(const_node_ptr_t node, layer_key_t layer) const
{
	if (node->is_end()) {
		if (node == this->root()) {
			return node;
		}
	}
	if (!node->is_end() && this->left(node)->count(layer)) {
		return find_nth(this->left(node), this->left(node)->count(layer)-1, layer);
	}
	while(true) {
		const_node_ptr_t par = this->parent(node);
		if (!par) {
			return this->left(this->begin());
		}
		if (node == this->left(par)) {
			node = par;
			continue;
		}
		if (node == this->right(par) && par->is_layer(layer)) {
			return par;
		}
		node = par;
		if (this->left(node)->count(layer)) {
			return find_nth(this->left(node), this->left(node)->count(layer)-1, layer);
		}
	}
}

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
	CountTree<N,A>::right_rotate(x);
	wide_count_update(x);
	wide_count_update(this->parent_(x));
}

template<class N, class A>
void Tiq::Tree::LayerTree<N,A>::transplant(node_ptr_t u, node_ptr_t v)
{
	CountTree<N,A>::transplant(u, v);
	std::vector<layer_key_t> keys = this->to_public_node(u)->values_.keys();

	if (!v->is_end()) {
		wide_count_update(v);
	}
	for (auto& k : keys) {
		upward_layer_count_update(v, k);
	}
}

template<class N, class A>
void Tiq::Tree::LayerTree<N,A>::upward_layer_count_update(node_ptr_t node, layer_key_t layer)
{
	do {
		layer_count_update(node, layer);
	} while((node = this->parent_(node)));
}

template<class N, class A>
void Tiq::Tree::LayerTree<N,A>::layer_count_update(node_ptr_t node, layer_key_t layer)
{
	if (node->is_end()) return;
	auto n = this->to_public_node(node);
	auto left = this->to_public_node(this->left_(node));
	auto right = this->to_public_node(this->right_(node));

	auto value = left->layers_.get(layer) + right->layers_.get(layer);
	if (n->is_min(layer)) {
		++value;
	}
	if (n->is_cut_at(layer)) {
		--value;
	}
	n->layers_.set(layer, value);
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

	if (!n->empty()) {
		layers.add(n->min_key(), 1);
	}
	if (n->is_cut()) {
		layers.add(n->cut_key(), -1);
	}
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
		if (left_count == count && node->is_layer(layer)) {
			return node;
		}
		if (left_count <= count) {
			count -= left_count;
			if (node->is_layer(layer)) {
				--count;
			}
			node = this->right(node);
		} else {
			node = this->left(node);
		}
	}
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::find_nth(size_t count, layer_key_t layer) const
{
	return find_nth(this->root(), count, layer);
}

template<class N, class A>
size_t Tiq::Tree::LayerTree<N,A>::find_index(const_node_ptr_t node, const_node_ptr_t parent) const
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
			if (par->is_layer(layer)) {
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
