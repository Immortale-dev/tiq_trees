#ifndef TQ_LAYER_TREE_H_
#define TQ_LAYER_TREE_H_

#include <cstdint>
#include <type_traits>
#include <utility>
#include <functional>
#include <vector>

#include "tq_tree.h"
#include "tq_count_tree.h"

namespace Tiq::Tree {
	template<class T>
	class LayerStatNode : public CountNode<T> {
		using value_t = intmax_t;
		template<class K, class A> friend class LayersCollection;

		private:
			value_t value_ = 0;
	};

	template <class K, class A = std::allocator<LayerStatNode<K>>>
	class LayersCollection : public CountTree<LayerStatNode<K>, A> {
		public:
			using value_t = intmax_t;
			using node_ptr_t = InternalNode*;
			using key_t = K;

			void set(key_t key, value_t value);
			void unset(key_t key);
			void add(key_t key, value_t value);
			void merge(const LayersCollection<K,A>& collection);
			value_t get(key_t key);
			value_t count(key_t key);

		protected:
			void calc_count(node_ptr_t x) override;

		private:
			LayerStatNode<K>* bs_find(K key);
	};

	template<class K, class T>
	class ValueStatNode : public Node<T> {
		template<class KK, class TT> friend class ValuesCollection;
		public:
			K key() { return key_; }

		private:
			K key_;
	};

	template<class K, class T>
	class ValuesCollection : public Tree<ValueStatNode<K,T>> {
		public:
			void set(K key, T value);
			void unset(K key);
			T& get(K key);
			T& get();
			bool has(K key);
			bool contains(K key);
			std::vector<K> keys();
			std::vector<K> cut(K key);

		private:
			ValueStatNode<K,T>* bs_find(K key);
	};

	template<class T, class K>
	class LayerNode : public CountNode<T> {
		template<class N, class A> friend class LayerTree;
		public:
			using value_type = T;
			using layer_key_t = K;

			using CountNode<T>::count;
			size_t count(layer_key_t key) { return layers_.count(key); }
			bool is_layer(layer_key_t layer) { return values_.has(layer) && !is_cut(layer); }
			bool has_layer(layer_key_t layer) { return values_.contains(layer) || is_cut_at(layer); }
			T& data() { return values_.get();  }
			T& data(layer_key_t key){ return values_.get(key); }
			layer_key_t cut_key() { return cut_key_; }
			bool is_cut() { return is_cut_; }
			bool is_cut(layer_key_t layer) { return is_cut_ && cut_key_ < layer; }
			bool is_cut_at(layer_key_t layer) { return is_cut_ && cut_key_ == layer; }

		protected:
			bool is_cut_ = false;
			layer_key_t cut_key_;
			ValuesCollection<K,T> values_;
			LayersCollection<K> layers_;
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
