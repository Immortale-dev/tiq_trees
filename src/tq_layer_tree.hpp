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
void Tiq::Tree::LayersCollection<K,T,A>::set(key_t key, value_t value)
{
	auto node = this->bs_find(key);
	node->key_ = key;
	this->insert(node, value);
}

template<class K, class T, class A>
void Tiq::Tree::LayersCollection<K,T,A>::unset(key_t key)
{
	auto node = this->bs_find(key);
	if (!node->is_end()) {
		this->erase(node);
	}
}

template<class K, class T, class A>
typename Tiq::Tree::LayersCollection<K,T,A>::value_t Tiq::Tree::LayersCollection<K,T,A>::get(key_t key) const
{
	auto node = this->bs_find(key);

	if (node->is_end()) return value_t{};
	return node->data();
}

template<class K, class T, class A>
typename Tiq::Tree::LayersCollection<K,T,A>::value_t Tiq::Tree::LayersCollection<K,T,A>::count(key_t key) const
{
	auto node = this->bs_find_floor(key);

	if (!node || node->is_end()) return value_t{};

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
const typename Tiq::Tree::ValuesCollection<K,T,A>::value_t* Tiq::Tree::ValuesCollection<K,T,A>::get(key_t key) const
{
	auto node = this->bs_find_floor(key);

	if (node && !node->is_end()) {
		return &(node->data());
	}

	return nullptr;
}

template<class K, class T, class A>
const typename Tiq::Tree::ValuesCollection<K,T,A>::value_t* Tiq::Tree::ValuesCollection<K,T,A>::get() const
{
	auto node = this->parent(this->end());

	if (node && !node->is_end()) {
		return &(node->data());
	}

	return nullptr;
}

template<class K, class T, class A>
bool Tiq::Tree::ValuesCollection<K,T,A>::has(key_t key) const
{
	auto node = this->bs_find_floor(key);
	return (node && !node->is_end());
}

template<class K, class T, class A>
bool Tiq::Tree::ValuesCollection<K,T,A>::has() const
{
	auto node = this->parent(this->end());
	return (node && !node->is_end());
}


template<class K, class T, class A>
bool Tiq::Tree::ValuesCollection<K,T,A>::contains(key_t key) const
{
	auto node = this->bs_find(key);
	return (node && !node->is_end());
}

template<class K, class T, class A>
Tiq::Tree::ValuesCollection<K,T,A>* Tiq::Tree::ValuesCollection<K,T,A>::get_tree() const
{
	return const_cast<ValuesCollection<K,T,A>*>(this);
}

// LayerNode

template<class K, class T>
size_t Tiq::Tree::LayerNode<K,T>::count(layer_key_t key) const
{
	return insert_layers_.count(key) - erase_layers_.count(key);
}

template<class K, class T>
const typename Tiq::Tree::LayerNode<K,T>::value_type& Tiq::Tree::LayerNode<K,T>::data() const
{
	if (has_branch_end()) {
		return data(*branch_end());
	}

	auto value = inserts_.get();
	if (!value) {
		throw std::logic_error("no data found");
	}
	return *value;
}

template<class K, class T>
const typename Tiq::Tree::LayerNode<K,T>::value_type& Tiq::Tree::LayerNode<K,T>::data(layer_key_t key) const
{
	if(!has_data(key)){
		throw std::logic_error("no data found");
	}
	return *(inserts_.get(key));
}

template<class K, class T>
bool Tiq::Tree::LayerNode<K,T>::has_branch_begin() const
{
	return inserts_.size() && (!erases_.size() || inserts_.begin()->key() < erases_.begin()->key());
}

template<class K, class T>
bool Tiq::Tree::LayerNode<K,T>::has_branch_end() const
{
	return has_branch_begin() && erases_.size();
}

template<class K, class T>
bool Tiq::Tree::LayerNode<K,T>::is_branch_begin(layer_key_t layer) const
{
	return has_branch_begin() && *branch_begin() == layer;
}

template<class K, class T>
bool Tiq::Tree::LayerNode<K,T>::is_branch_end(layer_key_t layer) const
{
	return has_branch_end() && *branch_end() == layer;
}

template<class K, class T>
bool Tiq::Tree::LayerNode<K,T>::has_branch(layer_key_t layer) const
{
	return inserts_.contains(layer) || erases_.contains(layer);
}

template<class K, class T>
bool Tiq::Tree::LayerNode<K,T>::has_data(layer_key_t layer) const
{
	return inserts_.has(layer) && !erases_.has(layer);
}

template<class K, class T>
const typename Tiq::Tree::LayerNode<K,T>::layer_key_t* Tiq::Tree::LayerNode<K,T>::branch_begin() const
{
	if (has_branch_begin()) {
		return &(inserts_.begin()->key());
	}
	return nullptr;
}

template<class K, class T>
const typename Tiq::Tree::LayerNode<K,T>::layer_key_t* Tiq::Tree::LayerNode<K,T>::branch_end() const
{
	if (has_branch_end()) {
		return &(erases_.begin()->key());
	}
	return nullptr;
}

template<class K, class T>
size_t Tiq::Tree::LayerNode<K,T>::size() const
{
	return inserts_.size() + erases_.size();
}

template<class K, class T>
typename Tiq::Tree::LayerNode<K,T>::LayerVector Tiq::Tree::LayerNode<K,T>::keys() const
{
	auto insert_node = inserts_.begin();
	auto erase_node = erases_.begin();
	LayerVector ret;
	while (!insert_node->is_end() || !erase_node->is_end()) {
		if (!erase_node->is_end() && (insert_node->is_end() || erase_node->key() < insert_node->key())) {
			ret.push_back(erase_node->key());
			erase_node = erases_.find_next(erase_node);
			continue;
		}
		if (!insert_node->is_end() && (erase_node->is_end() || insert_node->key() < erase_node->key())) {
			ret.push_back(insert_node->key());
			insert_node = inserts_.find_next(insert_node);
			continue;
		}
	}
	return ret;
}

template<class K, class T>
typename Tiq::Tree::LayerNode<K,T>::LayerVector Tiq::Tree::LayerNode<K,T>::insert_(layer_key_t layer, value_type& value)
{
	LayerRange range = get_range();

	inserts_.set(layer, value);
	erases_.unset(layer);

	return merge_ranges(range, get_range());
}

template<class K, class T>
typename Tiq::Tree::LayerNode<K,T>::LayerVector Tiq::Tree::LayerNode<K,T>::erase_(layer_key_t layer)
{
	LayerRange range = get_range();

	inserts_.unset(layer);
	erases_.set(layer, true);

	return merge_ranges(range, get_range());
}

template<class K, class T>
typename Tiq::Tree::LayerNode<K,T>::LayerVector Tiq::Tree::LayerNode<K,T>::remove_(layer_key_t layer)
{
	LayerRange range = get_range();

	inserts_.unset(layer);
	erases_.unset(layer);

	return merge_ranges(range, get_range());
}

template<class K, class T>
typename Tiq::Tree::LayerNode<K,T>::LayerVector Tiq::Tree::LayerNode<K,T>::clear_()
{
	LayerRange range = get_range();

	inserts_.clear();
	erases_.clear();

	return merge_ranges(range, get_range());
}

template<class K, class T>
typename Tiq::Tree::LayerNode<K,T>::LayerRange Tiq::Tree::LayerNode<K,T>::get_range() const
{
	LayerRange ret;
	if (has_branch_begin()) {
		ret.begin = *branch_begin();
	}
	if (has_branch_end()) {
		ret.end = *branch_end();
	}
	return ret;
}

template<class K, class T>
typename Tiq::Tree::LayerNode<K,T>::LayerVector Tiq::Tree::LayerNode<K,T>::merge_ranges(LayerRange r1, LayerRange r2) const
{
	LayerVector ret;
	if (r1.begin && (!r2.begin || r1.begin.value() != r2.begin.value())) {
		ret.push_back(r1.begin.value());
	}
	if (r2.begin && (!r1.begin || r1.begin.value() != r2.begin.value())) {
		ret.push_back(r2.begin.value());
	}
	if (r1.end && (!r2.end || r1.end.value() != r2.end.value())) {
		ret.push_back(r1.end.value());
	}
	if (r2.end && (!r1.end || r1.end.value() != r2.end.value())) {
		ret.push_back(r2.end.value());
	}
	return ret;
}

template<class K, class T>
bool Tiq::Tree::LayerNode<K,T>::empty_() const
{
	return !inserts_.size() && !erases_.size();
}

// LayerTree

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::insert(const_node_ptr_t node, value_type data, layer_key_t layer)
{
	auto keys = node->insert_(layer, data);

	auto n = this->insert_(node);

	for (auto &key : keys) {
		upward_layer_count_update(n, key);
	}

	return this->to_public_node(n);
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::erase(const_node_ptr_t node, layer_key_t layer, bool remove)
{
	if (remove) {
		return this->remove(node, layer);
	}

	if (node->is_end()) {
		throw std::logic_error("cannot erase end node");
	}

	auto keys = node->erase_(layer);
	for (auto &key : keys) {
		upward_layer_count_update(node, key);
	}

	return node;
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::erase(const_node_ptr_t node)
{
	return remove(node);
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::remove(const_node_ptr_t node, layer_key_t layer)
{
	if (node->is_end()) {
		throw std::logic_error("cannot remove end node");
	}

	auto keys = node->remove_(layer);
	for (auto &key : keys) {
		upward_layer_count_update(node, key);
	}

	if (!node->empty_()) {
		return node;
	}

	auto n = this->erase_(node);
	return this->to_public_node(n);
}

template<class N, class A>
typename Tiq::Tree::LayerTree<N,A>::const_node_ptr_t Tiq::Tree::LayerTree<N,A>::remove(const_node_ptr_t node)
{
	if (node->is_end()) {
		throw std::logic_error("cannot remove end node");
	}

	auto keys = node->clear_();

	for (auto& key : keys) {
		upward_layer_count_update(node, key);
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
		if (node == this->left(par) && par->has_data(layer)) {
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
		if (node == this->right(par) && par->has_data(layer)) {
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
	auto range_u = this->to_public_node(u)->get_range();
	auto range_v = this->to_public_node(v)->get_range();

	auto merged = this->to_public_node(v)->merge_ranges(range_u, range_v);

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

	auto insert_value = left->insert_layers_.get(layer) + right->insert_layers_.get(layer);
	auto erase_value = left->erase_layers_.get(layer) + right->erase_layers_.get(layer);
	if (n->is_branch_begin(layer)) {
		++insert_value;
	}
	if (n->is_branch_end(layer)) {
		++erase_value;
	}
	n->insert_layers_.set(layer, insert_value);
	n->erase_layers_.set(layer, erase_value);
}

template<class N, class A>
void Tiq::Tree::LayerTree<N,A>::wide_count_update(node_ptr_t node)
{
	auto n = this->to_public_node(node);
	auto& insert_layers = n->insert_layers_;
	auto& erase_layers = n->erase_layers_;
	auto left = this->to_public_node(this->left_(node));
	auto right = this->to_public_node(this->right_(node));

	insert_layers.clear();
	insert_layers.merge(left->insert_layers_);
	insert_layers.merge(right->insert_layers_);

	erase_layers.clear();
	erase_layers.merge(left->erase_layers_);
	erase_layers.merge(right->erase_layers_);

	if (n->has_branch_begin()) {
		insert_layers.add(*(n->branch_begin()), 1);
	}
	if (n->has_branch_end()) {
		erase_layers.add(*(n->branch_end()), 1);
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
		if (left_count == count && node->has_data(layer)) {
			return node;
		}
		if (left_count <= count) {
			count -= left_count;
			if (node->has_data(layer)) {
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
			if (par->has_data(layer)) {
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
