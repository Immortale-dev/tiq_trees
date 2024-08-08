// BSTree

template<class T, class N>
typename Tiq::Tree::BSTree<T,N>::const_node_ptr_t Tiq::Tree::BSTree<T,N>::bs_find(key_t key) const
{
	auto tree = get_tree();
	auto node = tree->root();
	while(!node->is_end()) {
		if (key == node->key()) {
			// Item found.
			break;
		}
		if (key > node->key()) {
			node = tree->right(node);
		} else {
			node = tree->left(node);
		}
	}
	return node;
}

template<class T, class N>
typename Tiq::Tree::BSTree<T,N>::const_node_ptr_t Tiq::Tree::BSTree<T,N>::bs_find_floor(key_t key) const
{
	auto tree = get_tree();
	auto node = bs_find(key);
	if (node->is_end()) {
		node = tree->find_prev(node);
	}
	return node;
}

// LayersCollection

template<class K, class T, class A>
void Tiq::Tree::LayersCollection<K,T,A>::set(K key, value_t value)
{
	auto node = this->bs_find(key);
	node->key_ = key;
	this->insert(node, value);
}

template<class K, class T, class A>
void Tiq::Tree::LayersCollection<K,T,A>::unset(K key)
{
	auto node = this->bs_find(key);
	if (!node->is_end()) {
		this->erase(node);
	}
}

template<class K, class T, class A>
typename Tiq::Tree::LayersCollection<K,T,A>::value_t Tiq::Tree::LayersCollection<K,T,A>::get(K key)
{
	auto node = this->bs_find(key);

	if (node->is_end()) return 0;
	return node->data();
}

template<class K, class T, class A>
typename Tiq::Tree::LayersCollection<K,T,A>::value_t Tiq::Tree::LayersCollection<K,T,A>::count(K key)
{
	auto node = this->bs_find_floor(key);

	if (!node || node->is_end()) return 0;

	value_t value = this->left(node)->count() + node->data();
	while(node) {
		auto par = this->parent(node);
		if (par && this->right(par) == node) {
			value += this->left(par)->count() + par->data();
		}
		node = par;
	}

	return value;
}

template<class K, class T, class A>
void Tiq::Tree::LayersCollection<K,T,A>::add(key_t key, value_t value)
{
	auto node = this->bs_find(key);
	if (!node->is_end()) {
		this->insert(node, node->data() + value);
		return;
	}
	// Insert if doesn't exist.
	node->key_ = key;
	this->insert(node, value);
}

template<class K, class T, class A>
void Tiq::Tree::LayersCollection<K,T,A>::merge(const LayersCollection<K,T,A>& collection)
{
	auto min = collection.find_min();
	while (!min->is_end()) {
		this->add(min->key(), min->data());
		min = collection.find_next(min);
	}
}

template<class K, class T, class A>
void Tiq::Tree::LayersCollection<K,T,A>::calc_count(node_ptr_t x)
{
	CountTree<LayersCollectionNode<K,T>,A>::calc_count(x);

	if (x->is_end()) return;

	auto node = this->to_public_node(x);

	auto left = this->left(node);
	auto right = this->right(node);

	node->count_ = left->count() + right->count() + node->data();
}

template<class K, class T, class A>
Tiq::Tree::LayersCollection<K,T,A>* Tiq::Tree::LayersCollection<K,T,A>::get_tree() const
{
	return const_cast<LayersCollection<K,T,A>*>(this);
}

// ValuesCollection

template<class K, class T, class A>
void Tiq::Tree::ValuesCollection<K,T,A>::set(key_t key, value_t value)
{
	auto node = this->bs_find(key);
	node->key_ = key;
	this->insert(node, value);
}

template<class K, class T, class A>
void Tiq::Tree::ValuesCollection<K,T,A>::unset(key_t key)
{
	auto node = this->bs_find(key);
	if (!node->is_end()) {
		this->erase(node);
	}
}

template<class K, class T, class A>
typename Tiq::Tree::ValuesCollection<K,T,A>::value_t& Tiq::Tree::ValuesCollection<K,T,A>::get(K key)
{
	auto node = this->bs_find_floor(key);

	if (!node || node->is_end()) {
		throw std::logic_error("No data found");
	}

	return node->data();
}

template<class K, class T, class A>
typename Tiq::Tree::ValuesCollection<K,T,A>::value_t& Tiq::Tree::ValuesCollection<K,T,A>::get()
{
	auto node = this->parent(this->end());

	if (!node || node->is_end()) {
		throw std::logic_error("No data found");
	}

	return node->data();
}

template<class K, class T, class A>
typename Tiq::Tree::ValuesCollection<K,T,A>::key_t Tiq::Tree::ValuesCollection<K,T,A>::min()
{
	if (!this->size()) {
		throw std::logic_error("No data found");
	}

	return this->begin()->key();
}

template<class K, class T, class A>
bool Tiq::Tree::ValuesCollection<K,T,A>::is_min(key_t key)
{
	auto node = this->begin();
	return !node->is_end() && node->key() == key;
}

template<class K, class T, class A>
bool Tiq::Tree::ValuesCollection<K,T,A>::has(key_t key)
{
	auto node = this->bs_find_floor(key);
	return (node && !node->is_end());
}

template<class K, class T, class A>
bool Tiq::Tree::ValuesCollection<K,T,A>::has()
{
	auto node = this->parent(this->end());
	return (node && !node->is_end());
}


template<class K, class T, class A>
bool Tiq::Tree::ValuesCollection<K,T,A>::contains(key_t key)
{
	auto node = this->bs_find(key);
	return (node && !node->is_end());
}

template<class K, class T, class A>
std::vector<K> Tiq::Tree::ValuesCollection<K,T,A>::keys()
{
	std::vector<K> ret;
	auto b = this->begin();
	while(!b->is_end()) {
		ret.push_back(b->key());
		b = this->find_next(b);
	}
	return ret;
}

template<class K, class T, class A>
std::vector<K> Tiq::Tree::ValuesCollection<K,T,A>::cut(key_t key)
{
	auto node = this->bs_find(key);
	if (!node) {
		node = this->find_next(node);
	}
	std::vector<ValuesCollectionNode<K,T>*> delete_nodes;
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

template<class K, class T, class A>
Tiq::Tree::ValuesCollection<K,T,A>* Tiq::Tree::ValuesCollection<K,T,A>::get_tree() const
{
	return const_cast<ValuesCollection<K,T,A>*>(this);
}

// LayerTree

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::insert(const_node_ptr_t node, T data, layer_key_t layer)
{
	bool update_min = false;
	bool update_cut = false;
	layer_key_t min_key;
	if (!node->is_cut(layer) && !node->is_layer(layer) && !node->empty()) {
		update_min = true;
		min_key = node->min_key();
	}
	if (node->is_cut_at(layer)) {
		update_cut = true;
	}

	node->inserts_.set(layer, data);
	node->erases_.unset(layer);
	auto n = this->insert_(node);

	if (update_min) {
		upward_layer_count_update(n, min_key);
	}
	if (update_cut && node->is_cut()) {
		upward_layer_count_update(n, node->cut_key());
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

	bool update_min = false;
	bool update_cut = false;
	layer_key_t cut_key;
	if (remove) {
		if (node->is_min(layer) && !node->empty()) {
			update_min = true;
		}
		if (node->empty() && node->is_cut_at(layer)) {
			update_min = true;
		}
		if (node->is_cut_at(layer)) {
			update_cut = true;
		}

		node->inserts_.unset(layer);
		node->erases_.unset(layer);

		if (update_min && !node->empty()) {
			upward_layer_count_update(node, node->min_key());
		}
		if (update_cut && node->is_cut()) {
			upward_layer_count_update(node, node->cut_key());
		}
		upward_layer_count_update(node, layer);
	} else {
		if (!node->is_cut(layer) && node->is_cut()) {
			update_cut = true;
			cut_key = node->cut_key();
		}
		if (!node->empty() && (!node->is_layer(layer) || node->is_min(layer))) {
			update_min = true;
		}

		node->inserts_.unset(layer);
		node->erases_.set(layer, true);

		if (update_min && node->inserts_.size()) {
			upward_layer_count_update(node, node->min_key());
		}
		if (update_cut) {
			upward_layer_count_update(node, cut_key);
		}
		upward_layer_count_update(node, layer);
	}

	if (node->inserts_.size() || node->erases_.size()) {
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

	bool update_min = false;
	bool update_cut = false;
	layer_key_t min_key, cut_key;

	if (!node->empty()) {
		update_min = true;
		min_key = node->min_key();
	}
	if (node->is_cut()) {
		update_cut = true;
		cut_key = node->cut_key();
	}

	node->inserts_.clear();
	node->erases_.clear();
	if (update_min) {
		upward_layer_count_update(node, min_key);
	}
	if (update_cut) {
		upward_layer_count_update(node, cut_key);
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
	std::vector<layer_key_t> crit_u_keys = this->to_public_node(u)->crit_keys();
	std::vector<layer_key_t> crit_v_keys = this->to_public_node(v)->crit_keys();

	std::vector<layer_key_t> merged = crit_u_keys;
	size_t length = merged.size();
	for (auto &k : crit_v_keys) {
		size_t i;
		for (i=0;i<length;i++) {
			if (merged[i] == k) break;
		}
		if (i == length) {
			merged.push_back(k);
		}
	}

	CountTree<N,A>::transplant(u, v);

	if (!v->is_end()) {
		wide_count_update(v);
	}
	for (auto &k : merged) {
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
	if (!n->empty() && n->is_min(layer)) {
		++value;
	}
	if (!n->empty() && n->is_cut_at(layer)) {
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
	if (!n->empty() && n->is_cut()) {
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

template<class N, class A>
size_t Tiq::Tree::LayerTree<N,A>::size() const
{
	return CountTree<N,A>::size();
}

template<class N, class A>
size_t Tiq::Tree::LayerTree<N,A>::size(layer_key_t layer) const
{
	return this->root()->count(layer);
}
