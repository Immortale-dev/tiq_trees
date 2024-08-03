#ifndef TQ_LAYER_TREE_H_
#define TQ_LAYER_TREE_H_

#include <type_traits>
#include <utility>
#include <functional>

#include "tq_tree.h"
#include "tq_count_tree.h"

namespace Tiq::Tree {
	template<class T>
	class LayerStatNode : public CountNode<T> {
		template<class K, class A> friend class LayersCollection;

		private:
			size_t value_ = 0;
	};

	template <class K, class A = std::allocator<LayerStatNode<K>>>
	class LayersCollection : public CountTree<LayerStatNode<K>, A> {
		public:
			using node_ptr_t = InternalNode*;
			using key_t = K;

			void set(key_t key, size_t value);
			void unset(key_t key);
			void add(key_t key, long int value);
			void merge(const LayersCollection<K,A>& collection);
			size_t get(key_t key);
			size_t count(key_t key);

		protected:
			void calc_count(node_ptr_t x) override;

		private:
			LayerStatNode<K>* bs_find(K key);
	};

	template<class T, class K, class L = LayersCollection<K>>
	class LayerNode : public CountNode<T> {
		template<class N, class A> friend class LayerTree;
		public:
			using value_type = T;
			using layer_key_t = K;

			using CountNode<T>::count;
			size_t count(layer_key_t key) { return layers_.count(key); }
			layer_key_t layer() { return layer_; }
			bool is_layer(layer_key_t layer) { return layer_ <= layer; }

		protected:
			L layers_;
			layer_key_t layer_;
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
			const_node_ptr_t update(const_node_ptr_t node, T data);
			const_node_ptr_t erase(const_node_ptr_t node);
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
