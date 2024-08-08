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

			key_t key(){ return key_; }

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
			value_t& get(key_t key);
			value_t& get();
			key_t min();
			bool is_min(key_t key);
			bool has();
			bool has(key_t key);
			bool contains(key_t key);
			std::vector<key_t> keys();
			std::vector<key_t> cut(key_t key);

		protected:
			ValuesCollection<K,T,A>* get_tree() const override;
	};

	template<class T, class K>
	class LayerNode : public CountNode<T> {
		template<class N, class A> friend class LayerTree;
		public:
			using value_type = T;
			using layer_key_t = K;

			using CountNode<T>::count;
			size_t count(layer_key_t key) { return insert_layers_.count(key) - erase_layers_.count(key); }
			bool is_layer(layer_key_t layer) { return inserts_.has(layer) && !erases_.has(layer); }
			bool has_layer(layer_key_t layer) { return inserts_.contains(layer) || erases_.contains(layer); }
			T& data() { return is_cut() ? inserts_.get(cut_key()) : inserts_.get();  }
			T& data(layer_key_t key){ if(is_cut(key)){ throw std::logic_error("No data found"); }  return inserts_.get(key); }
			layer_key_t cut_key() { return erases_.min(); }
			bool is_cut() { return !!erases_.size(); }
			bool is_cut(layer_key_t layer) { return erases_.has(layer); }
			bool is_cut_at(layer_key_t layer) { return is_cut() && cut_key() == layer; }
			layer_key_t min_key() { return inserts_.min(); }
			bool is_min(layer_key_t key) { return inserts_.is_min(key); };
			bool empty() { return !inserts_.size() || (erases_.size() && erases_.min() < inserts_.min()); }
			void debug() {
				std::cout << "I: ";
				auto b = inserts_.begin();
				while(!b->is_end()) {
					std::cout << b->key() << " ";
					b = inserts_.find_next(b);
				}
				std::cout << std::endl << "D: ";
				auto d = erases_.begin();
				while(!d->is_end()) {
					std::cout << d->key() << " ";
					d = erases_.find_next(d);
				}
				std::cout << std::endl << "L: ";
				auto l = insert_layers_.begin();
				while(!l->is_end()) {
					std::cout << l->data() << "=" << insert_layers_.get(l->data()) << " ";
					l = insert_layers_.find_next(l);
				}
				std::cout << std::endl;
			}

		protected:
			struct LayerRange {
				std::optional<layer_key_t> begin, end;
			};
			using LayerVector = std::vector<layer_key_t>;

			LayerVector insert_(layer_key_t layer, value_type& value) {
				LayerRange range = get_range();

				inserts_.set(layer, value);
				erases_.unset(layer);

				return merge_ranges(range, get_range());
			}
			LayerVector erase_(layer_key_t layer) {
				LayerRange range = get_range();

				inserts_.unset(layer);
				erases_.set(layer, true);

				return merge_ranges(range, get_range());
			}
			LayerVector remove_(layer_key_t layer) {
				LayerRange range = get_range();

				inserts_.unset(layer);
				erases_.unset(layer);

				return merge_ranges(range, get_range());
			}
			LayerVector clear_() {
				LayerRange range = get_range();

				inserts_.clear();
				erases_.clear();

				return merge_ranges(range, get_range());
			}

			LayerRange get_range() {
				LayerRange ret;
				if (!empty()) {
					ret.begin = min_key();
				}
				if (!empty() && is_cut()) {
					ret.end = cut_key();
				}
				return ret;
			}
			LayerVector merge_ranges(LayerRange r1, LayerRange r2) {
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
			bool empty_() {
				return !inserts_.size() && !erases_.size();
			}

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
