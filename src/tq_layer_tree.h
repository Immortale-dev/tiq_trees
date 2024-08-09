#ifndef TQ_LAYER_TREE_H_
#define TQ_LAYER_TREE_H_

#include <cstdint>
#include <type_traits>
#include <utility>
#include <functional>
#include <vector>
#include <algorithm>
#include <optional>

#include "tq_tree.h"
#include "tq_count_tree.h"

namespace Tiq::Tree {
	using collection_value_t = size_t;

	template<class K>
	class KeyedNode {
		public:
			using key_t = K;

			const key_t& key(){ return key_; }

		protected:
			key_t key_;
	};

	template<class T, class N>
	class BSTree {
		using const_node_ptr_t = N*;
		using key_t = typename N::key_t;

		protected:
			const_node_ptr_t bs_find(key_t key) const;
			const_node_ptr_t bs_find_floor(key_t) const;
			virtual T* get_tree() const = 0;
	};

	template<class K, class T>
	class LayersCollectionNode : public CountNode<T>, public KeyedNode<K> {
		using value_t = T;
		template<class K_, class T_, class A_> friend class LayersCollection;
	};

	template <class K, class T = collection_value_t, class A = std::allocator<LayersCollectionNode<K,T>>>
	class LayersCollection : public CountTree<LayersCollectionNode<K,T>,A>, public BSTree<LayersCollection<K,T,A>, LayersCollectionNode<K,T>> {
		public:
			using public_node_t = LayersCollectionNode<K,T>;
			using node_ptr_t = InternalNode*;
			using const_node_ptr_t = public_node_t*;
			using value_t = typename public_node_t::value_t;
			using key_t = K;

			void set(key_t key, value_t value);
			void unset(key_t key);
			void add(key_t key, value_t value);
			void merge(const LayersCollection<K,T,A>& collection);
			value_t get(key_t key);
			value_t count(key_t key);

		protected:
			void calc_count(node_ptr_t x) override;
			LayersCollection<K,T,A>* get_tree() const override;
	};

	template<class K, class T>
	class ValuesCollectionNode : public Node<T>, public KeyedNode<K> {
		using value_t = T;
		template<class K_, class T_, class A_> friend class ValuesCollection;
	};

	template<class K, class T, class A = std::allocator<ValuesCollectionNode<K,T>>>
	class ValuesCollection : public Tree<ValuesCollectionNode<K,T>,A>, public BSTree<ValuesCollection<K,T,A>, ValuesCollectionNode<K,T>> {
		public:
			using public_node_t = ValuesCollectionNode<K,T>;
			using node_ptr_t = InternalNode*;
			using const_node_ptr_t = public_node_t*;
			using value_t = typename public_node_t::value_t;
			using key_t = K;

			void set(key_t key, value_t value);
			void unset(key_t key);
			value_t* get(key_t key);
			value_t* get();
			bool has();
			bool has(key_t key);
			bool contains(key_t key);

		protected:
			ValuesCollection<K,T,A>* get_tree() const override;
	};

	template<class K, class T>
	class LayerNode : public CountNode<T> {
		template<class N, class A> friend class LayerTree;
		public:
			using value_type = T;
			using layer_key_t = K;
			using LayerVector = std::vector<layer_key_t>;

			using CountNode<T>::count;
			size_t count(layer_key_t key);
			T& data();
			T& data(layer_key_t key);
			bool has_branch_begin();
			bool has_branch_end();
			bool is_branch_begin(layer_key_t layer);
			bool is_branch_end(layer_key_t layer);
			bool has_branch(layer_key_t layer);
			bool has_data(layer_key_t layer);
			const layer_key_t* branch_begin();
			const layer_key_t* branch_end();
			size_t size();
			LayerVector keys();

		protected:
			struct LayerRange {
				std::optional<layer_key_t> begin, end;
			};

			LayerVector insert_(layer_key_t layer, value_type& value);
			LayerVector erase_(layer_key_t layer);
			LayerVector remove_(layer_key_t layer);
			LayerVector clear_();
			LayerRange get_range();
			LayerVector merge_ranges(LayerRange r1, LayerRange r2);
			bool empty_();

			ValuesCollection<K,T> inserts_;
			ValuesCollection<K,bool> erases_;
			LayersCollection<K> insert_layers_;
			LayersCollection<K> erase_layers_;
	};

	template<class N, class A = std::allocator<N>>
	class LayerTree : public CountTree<N, A> {
		using T = typename N::value_type;
		using node_ptr_t = InternalNode*;
		using const_node_ptr_t = N*;
		using layer_key_t = typename N::layer_key_t;
		using comparator_fn_t = std::function<int(T&)>;

		public:
			const_node_ptr_t insert(const_node_ptr_t node, T data, layer_key_t layer);
			const_node_ptr_t erase(const_node_ptr_t node);
			const_node_ptr_t erase(const_node_ptr_t node, layer_key_t layer, bool remove = false);
			const_node_ptr_t find(comparator_fn_t comp) const;
			const_node_ptr_t find(const_node_ptr_t node, comparator_fn_t comp) const;
			const_node_ptr_t find_min(const_node_ptr_t node = nullptr) const;
			const_node_ptr_t find_max(const_node_ptr_t node = nullptr) const;
			const_node_ptr_t find_min(const_node_ptr_t node, layer_key_t layer) const;
			const_node_ptr_t find_max(const_node_ptr_t node, layer_key_t layer) const;
			const_node_ptr_t find_min(layer_key_t layer) const;
			const_node_ptr_t find_max(layer_key_t layer) const;
			const_node_ptr_t find_next(const_node_ptr_t node) const;
			const_node_ptr_t find_prev(const_node_ptr_t node) const;
			const_node_ptr_t find_next(const_node_ptr_t node, layer_key_t layer) const;
			const_node_ptr_t find_prev(const_node_ptr_t node, layer_key_t layer) const;
			const_node_ptr_t find_nth(const_node_ptr_t node, size_t count) const;
			const_node_ptr_t find_nth(size_t count) const;
			const_node_ptr_t find_nth(const_node_ptr_t node, size_t count, layer_key_t layer) const;
			const_node_ptr_t find_nth(size_t count, layer_key_t layer) const;
			size_t find_index(const_node_ptr_t node, const_node_ptr_t parent = nullptr) const;
			size_t find_index(const_node_ptr_t node, const_node_ptr_t parent, layer_key_t layer) const;
			size_t find_index(const_node_ptr_t node, layer_key_t layer) const;
			size_t size() const;
			size_t size(layer_key_t layer) const;

		protected:
			void left_rotate(node_ptr_t x) override;
			void right_rotate(node_ptr_t x) override;
			void transplant(node_ptr_t u, node_ptr_t v) override;

		private:
			void upward_layer_count_update(node_ptr_t node, layer_key_t layer);
			void layer_count_update(node_ptr_t node, layer_key_t layer);
			void wide_count_update(node_ptr_t node);
	};
}

#include "tq_layer_tree.hpp"

#endif // TQ_LAYER_TREE_H_
