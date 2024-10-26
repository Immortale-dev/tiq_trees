// BSTree

template<class T, class N>
typename tiq::tree::detail::BSTree<T,N>::node_ptr_t tiq::tree::detail::BSTree<T,N>::bs_find(key_t key) const
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
typename tiq::tree::detail::BSTree<T,N>::node_ptr_t tiq::tree::detail::BSTree<T,N>::bs_find_floor(key_t key) const
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
void tiq::tree::detail::LayersCollection<K,T,A>::set(key_t key, value_t value)
{
	auto node = this->bs_find(key);
	node->key_ = key;
	this->insert(node, value);
}

template<class K, class T, class A>
void tiq::tree::detail::LayersCollection<K,T,A>::unset(key_t key)
{
	auto node = this->bs_find(key);
	if (!node->is_end()) {
		this->erase(node);
	}
}

template<class K, class T, class A>
typename tiq::tree::detail::LayersCollection<K,T,A>::value_t tiq::tree::detail::LayersCollection<K,T,A>::get(key_t key) const
{
	auto node = this->bs_find(key);

	if (node->is_end()) return value_t{};
	return node->data();
}

template<class K, class T, class A>
typename tiq::tree::detail::LayersCollection<K,T,A>::value_t tiq::tree::detail::LayersCollection<K,T,A>::count(key_t key) const
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
void tiq::tree::detail::LayersCollection<K,T,A>::add(key_t key, value_t value)
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
void tiq::tree::detail::LayersCollection<K,T,A>::merge(const LayersCollection<K,T,A>& collection)
{
	auto min = collection.find_min();
	while (!min->is_end()) {
		this->add(min->key(), min->data());
		min = collection.find_next(min);
	}
}

template<class K, class T, class A>
void tiq::tree::detail::LayersCollection<K,T,A>::calc_count(internal_node_ptr_t x)
{
	CountTree<LayersCollectionNode<K,T>,A>::calc_count(x);

	if (x->is_end()) return;

	auto node = this->to_public_node(x);

	auto left = this->left(node);
	auto right = this->right(node);

	node->count_ = left->count() + right->count() + node->data();
}

template<class K, class T, class A>
tiq::tree::detail::LayersCollection<K,T,A>* tiq::tree::detail::LayersCollection<K,T,A>::get_tree() const
{
	return const_cast<LayersCollection<K,T,A>*>(this);
}

// ValuesCollection

template<class K, class T, class A>
void tiq::tree::detail::ValuesCollection<K,T,A>::set(key_t key, value_t value)
{
	auto node = this->bs_find(key);
	node->key_ = key;
	this->insert(node, value);
}

template<class K, class T, class A>
void tiq::tree::detail::ValuesCollection<K,T,A>::unset(key_t key)
{
	auto node = this->bs_find(key);
	if (!node->is_end()) {
		this->erase(node);
	}
}

template<class K, class T, class A>
const typename tiq::tree::detail::ValuesCollection<K,T,A>::value_t* tiq::tree::detail::ValuesCollection<K,T,A>::get(key_t key) const
{
	auto node = this->bs_find_floor(key);

	if (node && !node->is_end()) {
		return &(node->data());
	}

	return nullptr;
}

template<class K, class T, class A>
const typename tiq::tree::detail::ValuesCollection<K,T,A>::value_t* tiq::tree::detail::ValuesCollection<K,T,A>::get() const
{
	auto node = this->parent(this->end());

	if (node && !node->is_end()) {
		return &(node->data());
	}

	return nullptr;
}

template<class K, class T, class A>
const typename tiq::tree::detail::ValuesCollection<K,T,A>::value_t* tiq::tree::detail::ValuesCollection<K,T,A>::at(key_t key) const
{
	auto node = this->bs_find(key);

	if (node && !node->is_end()) {
		return &(node->data());
	}

	return nullptr;
}

template<class K, class T, class A>
bool tiq::tree::detail::ValuesCollection<K,T,A>::has(key_t key) const
{
	auto node = this->bs_find_floor(key);
	return (node && !node->is_end());
}

template<class K, class T, class A>
bool tiq::tree::detail::ValuesCollection<K,T,A>::has() const
{
	auto node = this->parent(this->end());
	return (node && !node->is_end());
}


template<class K, class T, class A>
bool tiq::tree::detail::ValuesCollection<K,T,A>::contains(key_t key) const
{
	auto node = this->bs_find(key);
	return (node && !node->is_end());
}

template<class K, class T, class A>
tiq::tree::detail::ValuesCollection<K,T,A>* tiq::tree::detail::ValuesCollection<K,T,A>::get_tree() const
{
	return const_cast<ValuesCollection<K,T,A>*>(this);
}

// BranchNode

template<class K, class T>
size_t tiq::tree::BranchNode<K,T>::count(branch_type branch, bool include_erased) const
{
//	std::cout << "B: " << branch << "; ie: " << include_erased << "; cnt: " << insert_layers_.count(branch) << "; cnt_e: " << erase_layers_.count(branch) << std::endl;
	return insert_layers_.count(branch) - (!include_erased ? erase_layers_.count(branch) : 0);
}

template<class K, class T>
const typename tiq::tree::BranchNode<K,T>::value_type* tiq::tree::BranchNode<K,T>::data_at(branch_type branch) const
{
	return inserts_.at(branch);
}

template<class K, class T>
const typename tiq::tree::BranchNode<K,T>::value_type& tiq::tree::BranchNode<K,T>::data() const
{
	const value_type* value = inserts_.get();
	if (!value || has_branch_end()) {
		throw std::logic_error("no data found");
	}
	return *value;
}

template<class K, class T>
const typename tiq::tree::BranchNode<K,T>::value_type& tiq::tree::BranchNode<K,T>::data(branch_type branch) const
{
	if(!has_data(branch)){
		throw std::logic_error("no data found");
	}
	return *(inserts_.get(branch));
}

template<class K, class T>
bool tiq::tree::BranchNode<K,T>::has_branch_begin() const
{
	return inserts_.size() && (!erases_.size() || inserts_.begin()->key() < erases_.begin()->key());
}

template<class K, class T>
bool tiq::tree::BranchNode<K,T>::has_branch_end() const
{
	return erases_.size();
}

template<class K, class T>
bool tiq::tree::BranchNode<K,T>::is_branch_begin(branch_type branch) const
{
	return has_branch_begin() && *branch_begin() == branch;
}

template<class K, class T>
bool tiq::tree::BranchNode<K,T>::is_branch_end(branch_type branch) const
{
	return has_branch_end() && *branch_end() == branch;
}

template<class K, class T>
bool tiq::tree::BranchNode<K,T>::has_branch(branch_type branch) const
{
	return inserts_.contains(branch) || erases_.contains(branch);
}

template<class K, class T>
bool tiq::tree::BranchNode<K,T>::has_data() const
{
	const value_type* value;
	if (has_branch_end()) {
		value = inserts_.get(*branch_end());
	} else {
		value = inserts_.get();
	}

	return !!value;
}

template<class K, class T>
bool tiq::tree::BranchNode<K,T>::has_data(branch_type branch, bool include_erased) const
{
	if (include_erased) {
		return inserts_.has(branch) || erases_.has(branch);
	} else {
		return inserts_.has(branch) && !erases_.has(branch);
	}
}

template<class K, class T>
const typename tiq::tree::BranchNode<K,T>::branch_type* tiq::tree::BranchNode<K,T>::branch_begin() const
{
	if (has_branch_begin()) {
		return &(inserts_.begin()->key());
	}
	return nullptr;
}

template<class K, class T>
const typename tiq::tree::BranchNode<K,T>::branch_type* tiq::tree::BranchNode<K,T>::branch_end() const
{
	if (has_branch_end()) {
		return &(erases_.begin()->key());
	}
	return nullptr;
}

template<class K, class T>
size_t tiq::tree::BranchNode<K,T>::size() const
{
	return inserts_.size() + erases_.size();
}

template<class K, class T>
typename tiq::tree::BranchNode<K,T>::BranchVector tiq::tree::BranchNode<K,T>::branches() const
{
	auto insert_node = inserts_.begin();
	auto erase_node = erases_.begin();
	BranchVector ret;
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
typename tiq::tree::BranchNode<K,T>::BranchVector tiq::tree::BranchNode<K,T>::insert_(branch_type branch, value_type& value)
{
	BranchRange range = get_range();

	inserts_.set(branch, value);
	erases_.unset(branch);

	return merge_ranges(range, get_range());
}

template<class K, class T>
typename tiq::tree::BranchNode<K,T>::BranchVector tiq::tree::BranchNode<K,T>::erase_(branch_type branch)
{
	BranchRange range = get_range();

	inserts_.unset(branch);
	erases_.set(branch, true);

	return merge_ranges(range, get_range());
}

template<class K, class T>
typename tiq::tree::BranchNode<K,T>::BranchVector tiq::tree::BranchNode<K,T>::remove_(branch_type branch)
{
	BranchRange range = get_range();

	inserts_.unset(branch);
	erases_.unset(branch);

	return merge_ranges(range, get_range());
}

template<class K, class T>
typename tiq::tree::BranchNode<K,T>::BranchVector tiq::tree::BranchNode<K,T>::clear_()
{
	BranchRange range = get_range();

	inserts_.clear();
	erases_.clear();

	return merge_ranges(range, get_range());
}

template<class K, class T>
typename tiq::tree::BranchNode<K,T>::BranchRange tiq::tree::BranchNode<K,T>::get_range() const
{
	BranchRange ret;
	if (has_branch_begin()) {
		ret.begin = *branch_begin();
	}
	if (has_branch_end()) {
		ret.end = *branch_end();
	}
	return ret;
}

template<class K, class T>
typename tiq::tree::BranchNode<K,T>::BranchVector tiq::tree::BranchNode<K,T>::merge_ranges(BranchRange r1, BranchRange r2) const
{
	BranchVector ret;
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
bool tiq::tree::BranchNode<K,T>::empty_() const
{
	return !inserts_.size() && !erases_.size();
}

// BranchTree

template<class N, class A>
typename tiq::tree::BranchTree<N,A>::node_ptr_t tiq::tree::BranchTree<N,A>::insert(node_ptr_t node, value_type data, branch_type branch)
{
	auto keys = node->insert_(branch, data);

	auto n = this->insert_(node);

	for (auto &k : keys) {
		upward_layer_count_update(n, k);
	}

	return this->to_public_node(n);
}

template<class N, class A>
typename tiq::tree::BranchTree<N,A>::node_ptr_t tiq::tree::BranchTree<N,A>::erase(node_ptr_t node, branch_type branch, bool remove)
{
	if (remove) {
		return this->remove(node, branch);
	}

	if (node->is_end()) {
		throw std::logic_error("cannot erase end node");
	}

	auto keys = node->erase_(branch);
	for (auto &k : keys) {
		upward_layer_count_update(node, k);
	}

	return node;
}

template<class N, class A>
typename tiq::tree::BranchTree<N,A>::node_ptr_t tiq::tree::BranchTree<N,A>::erase(node_ptr_t node)
{
	return remove(node);
}

template<class N, class A>
typename tiq::tree::BranchTree<N,A>::node_ptr_t tiq::tree::BranchTree<N,A>::remove(node_ptr_t node, branch_type branch)
{
	if (node->is_end()) {
		throw std::logic_error("cannot remove end node");
	}

	auto keys = node->remove_(branch);
	for (auto &k : keys) {
		upward_layer_count_update(node, k);
	}

	if (!node->empty_()) {
		return node;
	}

	auto n = this->erase_(node);
	return this->to_public_node(n);
}

template<class N, class A>
typename tiq::tree::BranchTree<N,A>::node_ptr_t tiq::tree::BranchTree<N,A>::remove(node_ptr_t node)
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
typename tiq::tree::BranchTree<N,A>::node_ptr_t tiq::tree::BranchTree<N,A>::find(comparator_fn_t comp) const
{
	return CountTree<N,A>::find(comp);
}

template<class N, class A>
typename tiq::tree::BranchTree<N,A>::node_ptr_t tiq::tree::BranchTree<N,A>::find(node_ptr_t node, comparator_fn_t comp) const
{
	return CountTree<N,A>::find(node, comp);
}

template<class N, class A>
typename tiq::tree::BranchTree<N,A>::node_ptr_t tiq::tree::BranchTree<N,A>::find_min(node_ptr_t node) const
{
	return CountTree<N,A>::find_min(node);
}

template<class N, class A>
typename tiq::tree::BranchTree<N,A>::node_ptr_t tiq::tree::BranchTree<N,A>::find_max(node_ptr_t node) const
{
	return CountTree<N,A>::find_max(node);
}

template<class N, class A>
typename tiq::tree::BranchTree<N,A>::node_ptr_t tiq::tree::BranchTree<N,A>::find_min(node_ptr_t node, branch_type branch, bool search_erased) const
{
	return this->find_nth(node, 0, branch, search_erased);
}

template<class N, class A>
typename tiq::tree::BranchTree<N,A>::node_ptr_t tiq::tree::BranchTree<N,A>::find_max(node_ptr_t node, branch_type branch, bool search_erased) const
{
	size_t size = node->count(branch, search_erased);
	return this->find_nth(node, size-1, branch, search_erased);
}

template<class N, class A>
typename tiq::tree::BranchTree<N,A>::node_ptr_t tiq::tree::BranchTree<N,A>::find_min(branch_type branch, bool search_erased) const
{
	return find_min(this->root(), branch, search_erased);
}

template<class N, class A>
typename tiq::tree::BranchTree<N,A>::node_ptr_t tiq::tree::BranchTree<N,A>::find_max(branch_type branch, bool search_erased) const
{
	return find_max(this->root(), branch, search_erased);
}

template<class N, class A>
typename tiq::tree::BranchTree<N,A>::node_ptr_t tiq::tree::BranchTree<N,A>::find_next(node_ptr_t node) const
{
	return CountTree<N,A>::find_next(node);
}

template<class N, class A>
typename tiq::tree::BranchTree<N,A>::node_ptr_t tiq::tree::BranchTree<N,A>::find_prev(node_ptr_t node) const
{
	return CountTree<N,A>::find_prev(node);
}

template<class N, class A>
typename tiq::tree::BranchTree<N,A>::node_ptr_t tiq::tree::BranchTree<N,A>::find_next(node_ptr_t node, branch_type branch, bool search_erased) const
{
	if (node->is_end()) {
		if (node == this->root()) {
			return node;
		}
	}
	if (!node->is_end() && this->right(node)->count(branch, search_erased)) {
		return find_nth(this->right(node), 0, branch, search_erased);
	}
	while(true) {
		node_ptr_t par = this->parent(node);
		if (!par) {
			return this->end();
		}
		if (node == this->right(par)) {
			node = par;
			continue;
		}
		if (node == this->left(par) && par->has_data(branch, search_erased)) {
			return par;
		}
		node = par;
		if (this->right(node)->count(branch, search_erased)) {
			return find_nth(this->right(node), 0, branch, search_erased);
		}
	}
}

template<class N, class A>
typename tiq::tree::BranchTree<N,A>::node_ptr_t tiq::tree::BranchTree<N,A>::find_prev(node_ptr_t node, branch_type branch, bool search_erased) const
{
	if (node->is_end()) {
		if (node == this->root()) {
			return node;
		}
	}
	if (!node->is_end() && this->left(node)->count(branch, search_erased)) {
		return find_nth(this->left(node), this->left(node)->count(branch, search_erased)-1, branch, search_erased);
	}
	while(true) {
		node_ptr_t par = this->parent(node);
		if (!par) {
			return this->left(this->begin());
		}
		if (node == this->left(par)) {
			node = par;
			continue;
		}
		if (node == this->right(par) && par->has_data(branch, search_erased)) {
			return par;
		}
		node = par;
		if (this->left(node)->count(branch, search_erased)) {
			return find_nth(this->left(node), this->left(node)->count(branch, search_erased)-1, branch, search_erased);
		}
	}
}

template<class N, class A>
void tiq::tree::BranchTree<N,A>::left_rotate(internal_node_ptr_t x)
{
	CountTree<N,A>::left_rotate(x);
	wide_count_update(x);
	wide_count_update(this->parent_(x));
}

template<class N, class A>
void tiq::tree::BranchTree<N,A>::right_rotate(internal_node_ptr_t x)
{
	CountTree<N,A>::right_rotate(x);
	wide_count_update(x);
	wide_count_update(this->parent_(x));
}

template<class N, class A>
void tiq::tree::BranchTree<N,A>::transplant(internal_node_ptr_t u, internal_node_ptr_t v)
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
void tiq::tree::BranchTree<N,A>::upward_layer_count_update(internal_node_ptr_t node, branch_type branch)
{
	do {
		layer_count_update(node, branch);
	} while((node = this->parent_(node)));
}

template<class N, class A>
void tiq::tree::BranchTree<N,A>::layer_count_update(internal_node_ptr_t node, branch_type branch)
{
	if (node->is_end()) return;
	auto n = this->to_public_node(node);
	auto left = this->to_public_node(this->left_(node));
	auto right = this->to_public_node(this->right_(node));

	auto insert_value = left->insert_layers_.get(branch) + right->insert_layers_.get(branch);
	auto erase_value = left->erase_layers_.get(branch) + right->erase_layers_.get(branch);
	if (n->is_branch_begin(branch) || (!n->has_branch_begin() && n->is_branch_end(branch))) {
		++insert_value;
	}
	if (n->is_branch_end(branch)) {
		++erase_value;
	}
	n->insert_layers_.set(branch, insert_value);
	n->erase_layers_.set(branch, erase_value);
}

template<class N, class A>
void tiq::tree::BranchTree<N,A>::wide_count_update(internal_node_ptr_t node)
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
	} else if (n->has_branch_end()) {
		insert_layers.add(*(n->branch_end()), 1);
	}
	if (n->has_branch_end()) {
		erase_layers.add(*(n->branch_end()), 1);
	}
}

template<class N, class A>
typename tiq::tree::BranchTree<N,A>::node_ptr_t tiq::tree::BranchTree<N,A>::find_nth(node_ptr_t node, size_t index) const
{
	return CountTree<N,A>::find_nth(node, index);
}

template<class N, class A>
typename tiq::tree::BranchTree<N,A>::node_ptr_t tiq::tree::BranchTree<N,A>::find_nth(size_t index) const
{
	return CountTree<N,A>::find_nth(index);
}

template<class N, class A>
typename tiq::tree::BranchTree<N,A>::node_ptr_t tiq::tree::BranchTree<N,A>::find_nth(node_ptr_t node, size_t index, branch_type branch, bool search_erased) const
{
	if (node->is_end()) {
		return node;
	}
	if (node->count(branch, search_erased) <= index) {
		return this->right(this->find_max(node));
	}
	while(true) {
		size_t left_count = this->left(node)->count(branch, search_erased);
		if (left_count == index && node->has_data(branch, search_erased)) {
			return node;
		}
		if (left_count <= index) {
			index -= left_count;
			if (node->has_data(branch, search_erased)) {
				--index;
			}
			node = this->right(node);
		} else {
			node = this->left(node);
		}
	}
}

template<class N, class A>
typename tiq::tree::BranchTree<N,A>::node_ptr_t tiq::tree::BranchTree<N,A>::find_nth(size_t index, branch_type branch, bool search_erased) const
{
	return find_nth(this->root(), index, branch, search_erased);
}

template<class N, class A>
size_t tiq::tree::BranchTree<N,A>::find_index(node_ptr_t node, node_ptr_t parent) const
{
	return CountTree<N,A>::find_index(node, parent);
}

template<class N, class A>
size_t tiq::tree::BranchTree<N,A>::find_index(node_ptr_t node, node_ptr_t parent, branch_type branch, bool search_erased) const
{
	size_t count = 0;
	if (!node->is_end()) {
		count += this->left(node)->count(branch, search_erased);
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
			count += this->left(par)->count(branch, search_erased);
			if (par->has_data(branch, search_erased)) {
				++count;
			}
		}
		node = par;
	}
	return count;
}

template<class N, class A>
size_t tiq::tree::BranchTree<N,A>::find_index(node_ptr_t node, branch_type branch, bool search_erased) const
{
	return find_index(node, this->root(), branch, search_erased);
}

template<class N, class A>
size_t tiq::tree::BranchTree<N,A>::size() const
{
	return CountTree<N,A>::size();
}

template<class N, class A>
size_t tiq::tree::BranchTree<N,A>::size(branch_type branch, bool search_erased) const
{
	return this->root()->count(branch, search_erased);
}
