#ifndef TQ_BRANCH_TREE_H_
#define TQ_BRANCH_TREE_H_

#include <cstdint>
#include <type_traits>
#include <utility>
#include <functional>
#include <vector>
#include <algorithm>
#include <optional>

#include "tq_tree.h"
#include "tq_count_tree.h"

namespace tiq::tree {
	namespace detail {
		template<class K>
		class KeyedNode {
			public:
				using key_t = K;

				const key_t& key() const { return key_; }

			protected:
				key_t key_;
		};

		template<class T, class N>
		class BSTree {
			using node_ptr_t = N*;
			using key_t = typename N::key_t;

			protected:
				node_ptr_t bs_find(key_t key) const;
				node_ptr_t bs_find_floor(key_t) const;
				virtual T* get_tree() const = 0;
		};

		template<class K, class T>
		class LayersCollectionNode : public CountNode<T>, public KeyedNode<K> {
			using value_t = T;
			template<class K_, class T_, class A_> friend class LayersCollection;
		};

		template <class K, class T = size_t, class A = std::allocator<LayersCollectionNode<K,T>>>
		class LayersCollection : public CountTree<LayersCollectionNode<K,T>,A>, public BSTree<LayersCollection<K,T,A>, LayersCollectionNode<K,T>> {
			public:
				using public_node_t = LayersCollectionNode<K,T>;
				using internal_node_ptr_t = InternalNode*;
				using node_ptr_t = public_node_t*;
				using value_t = typename public_node_t::value_t;
				using key_t = K;

				void set(key_t key, value_t value);
				void unset(key_t key);
				void add(key_t key, value_t value);
				void merge(const LayersCollection<K,T,A>& collection);
				value_t get(key_t key) const;
				value_t count(key_t key) const;

			protected:
				void calc_count(internal_node_ptr_t x) override;
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
				using internal_node_ptr_t = InternalNode*;
				using node_ptr_t = public_node_t*;
				using value_t = typename public_node_t::value_t;
				using key_t = K;

				void set(key_t key, value_t value);
				void unset(key_t key);
				const value_t* get(key_t key) const;
				const value_t* get() const;
				bool has() const;
				bool has(key_t key) const;
				bool contains(key_t key) const;

			protected:
				ValuesCollection<K,T,A>* get_tree() const override;
		};
	} // namespace tiq::tree::detail

	template<class K, class T>
	class BranchNode : public CountNode<T> {
		template<class N, class A> friend class BranchTree;
		public:
			using value_type = T;
			using branch_type = K;
			using BranchVector = std::vector<branch_type>;
			struct BranchRange {
				std::optional<branch_type> begin, end;
			};

			using CountNode<value_type>::count;
			size_t count(branch_type branch) const;
			const value_type& data() const;
			const value_type& data(branch_type branch) const;
			bool has_branch_begin() const;
			bool has_branch_end() const;
			bool is_branch_begin(branch_type branch) const;
			bool is_branch_end(branch_type granch) const;
			bool has_branch(branch_type branch) const;
			bool has_data() const;
			bool has_data(branch_type branch) const;
			const branch_type* branch_begin() const;
			const branch_type* branch_end() const;
			size_t size() const;
			BranchVector branches() const;

		protected:
			BranchVector insert_(branch_type branch, value_type& value);
			BranchVector erase_(branch_type branch);
			BranchVector remove_(branch_type branch);
			BranchVector clear_();
			BranchRange get_range() const;
			BranchVector merge_ranges(BranchRange r1, BranchRange r2) const;
			bool empty_() const;

			detail::ValuesCollection<branch_type,value_type> inserts_;
			detail::ValuesCollection<branch_type,bool> erases_;
			detail::LayersCollection<branch_type> insert_layers_;
			detail::LayersCollection<branch_type> erase_layers_;
	};

	template<class N, class A = std::allocator<N>>
	class BranchTree : public CountTree<N, A> {
		public:
			using value_type = typename N::value_type;
			using internal_node_ptr_t = InternalNode*;
			using node_ptr_t = N*;
			using branch_type = typename N::branch_type;
			using comparator_fn_t = std::function<int(const value_type&)>;

			node_ptr_t insert(node_ptr_t node, value_type data, branch_type branch);
			node_ptr_t erase(node_ptr_t node);
			node_ptr_t erase(node_ptr_t node, branch_type branch, bool remove = false);
			node_ptr_t remove(node_ptr_t node, branch_type branch);
			node_ptr_t remove(node_ptr_t node);
			node_ptr_t find(comparator_fn_t comp) const;
			node_ptr_t find(node_ptr_t node, comparator_fn_t comp) const;
			node_ptr_t find_min(node_ptr_t node = nullptr) const;
			node_ptr_t find_max(node_ptr_t node = nullptr) const;
			node_ptr_t find_min(node_ptr_t node, branch_type branch) const;
			node_ptr_t find_max(node_ptr_t node, branch_type branch) const;
			node_ptr_t find_min(branch_type branch) const;
			node_ptr_t find_max(branch_type branch) const;
			node_ptr_t find_next(node_ptr_t node) const;
			node_ptr_t find_prev(node_ptr_t node) const;
			node_ptr_t find_next(node_ptr_t node, branch_type branch) const;
			node_ptr_t find_prev(node_ptr_t node, branch_type branch) const;
			node_ptr_t find_nth(node_ptr_t node, size_t count) const;
			node_ptr_t find_nth(size_t count) const;
			node_ptr_t find_nth(node_ptr_t node, size_t count, branch_type branch) const;
			node_ptr_t find_nth(size_t count, branch_type branch) const;
			size_t find_index(node_ptr_t node, node_ptr_t parent = nullptr) const;
			size_t find_index(node_ptr_t node, node_ptr_t parent, branch_type branch) const;
			size_t find_index(node_ptr_t node, branch_type branch) const;
			size_t size() const;
			size_t size(branch_type branch) const;

		protected:
			void left_rotate(internal_node_ptr_t x) override;
			void right_rotate(internal_node_ptr_t x) override;
			void transplant(internal_node_ptr_t u, internal_node_ptr_t v) override;

		private:
			void upward_layer_count_update(internal_node_ptr_t node, branch_type branch);
			void layer_count_update(internal_node_ptr_t node, branch_type branch);
			void wide_count_update(internal_node_ptr_t node);
	};
} // namespace tiq::tree

#include "tq_branch_tree.hpp"

#endif // TQ_BRANCH_TREE_H_
