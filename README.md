# TIQ Trees

The repository provides C++ implementation of some useful and unique tree data structures.

## Table of Contest

* [Dependencies](#dependencies)
* [Build](#build)
* [Documentation](#documentation)
	* [class tiq::tree::Node<T>](#class-tiqtreenodet)
		* [const T& tiq::tree::Node<T>::data()](#const-t-tiqtreenodetdata)
		* [bool tiq::tree::Node<T>::is_end()](#bool-tiqtreenodetis_end)
	* [class tiq::tree::Tree<N,A>](#class-tiqtreetreena)
		* [node_ptr_t tiq::tree::Tree<N,T>::root()](#node_ptr_t-tiqtreetreentroot)
		* [node_ptr_t tiq::tree::Tree<N,T>::begin()](#node_ptr_t-tiqtreetreentbegin)
		* [node_ptr_t tiq::tree::Tree<N,T>::end()](#node_ptr_t-tiqtreetreentend)
		* [node_ptr_t tiq::tree::Tree<N,T>::find(comparator_fn_t comp)](#node_ptr_t-tiqtreetreentfindcomparator_fn_t-comp)
		* [node_ptr_t tiq::tree::Tree<N,T>::find(node_ptr_t node, comparator_fn_t comp)](#node_ptr_t-tiqtreetreentfindnode_ptr_t-node-comparator_fn_t-comp)
		* [node_ptr_t tiq::tree::Tree<N,T>::find_min(node_ptr_t node = nullptr)](#node_ptr_t-tiqtreetreentfind_minnode_ptr_t-node--nullptr)
		* [node_ptr_t tiq::tree::Tree<N,T>::find_max(node_ptr_t node = nullptr)](#node_ptr_t-tiqtreetreentfind_maxnode_ptr_t-node--nullptr)
		* [node_ptr_t tiq::tree::Tree<N,T>::find_next(node_ptr_t node)](#node_ptr_t-tiqtreetreentfind_nextnode_ptr_t-node)
		* [node_ptr_t tiq::tree::Tree<N,T>::find_prev(node_ptr_t node)](#node_ptr_t-tiqtreetreentfind_prevnode_ptr_t-node)
		* [node_ptr_t tiq::tree::Tree<N,T>::parent(node_ptr_t node)](#node_ptr_t-tiqtreetreentparentnode_ptr_t-node)
		* [node_ptr_t tiq::tree::Tree<N,T>::left(node_ptr_t node)](#node_ptr_t-tiqtreetreentleftnode_ptr_t-node)
		* [node_ptr_t tiq::tree::Tree<N,T>::right(node_ptr_t node)](#node_ptr_t-tiqtreetreentrightnode_ptr_t-node)
		* [node_ptr_t tiq::tree::Tree<N,T>::before(node_ptr_t node = nullptr)](#node_ptr_t-tiqtreetreentbeforenode_ptr_t-node--nullptr)
		* [node_ptr_t tiq::tree::Tree<N,T>::after(node_ptr_t node = nullptr)](#node_ptr_t-tiqtreetreentafternode_ptr_t-node--nullptr)
		* [node_ptr_t tiq::tree::Tree<N,T>::insert(node_ptr_t node, T data)](#node_ptr_t-tiqtreetreentinsertnode_ptr_t-node-t-data)
		* [node_ptr_t tiq::tree::Tree<N,T>::erase(node_ptr_t node)](#node_ptr_t-tiqtreetreenterasenode_ptr_t-node)
		* [void tiq::tree::Tree<N,T>::clear()](#void-tiqtreetreentclear)
		* [size_t tiq::tree::size()](#size_t-tiqtreesize)
	* [class tiq::tree::CountNode<T>](#class-tiqtreecountnodet)
		* [size_t tiq::tree::CountNode<T>::count()](#size_t-tiqtreecountnodetcount)
	* [class tiq::tree::CountTree<N,A>](#class-tiqtreecounttreena)
		* [node_ptr_t tiq::tree::CountTree<N,A>::find_nth(size_t count)](#node_ptr_t-tiqtreecounttreenafind_nthsize_t-count)
		* [node_ptr_t tiq::tree::CountTree<N,A>::find_nth(node_ptr_t node, size_t count)](#node_ptr_t-tiqtreecounttreenafind_nthnode_ptr_t-node-size_t-count)
		* [size_t tiq::tree::find_index(node_ptr_t node, node_ptr_t parent = nullptr)](#size_t-tiqtreefind_indexnode_ptr_t-node-node_ptr_t-parent--nullptr)
	* [class tiq::tree::BranchNode<K,T>](#class-tiqtreebranchnodekt)
		* [size_t tiq::tree::BranchNode<K,T>::count()](#size_t-tiqtreebranchnodektcount)
		* [size_t tiq::tree::BranchNode<K,T>::count(branch_type branch, bool include_erased=false)](#size_t-tiqtreebranchnodektcountbranch_type-branch-bool-include_erasedfalse)
		* [const value_type& tiq::tree::BranchNode<K,T>::data()](#const-value_type-tiqtreebranchnodektdata)
		* [const value_type& tiq::tree::BranchNode<K,T>::data(branch_type branch)](#const-value_type-tiqtreebranchnodektdatabranch_type-branch)
		* [const value_type* tiq::tree::BranchNode<K,T>::data_at(branch_type branch)](#const-value_type-tiqtreebranchnodektdata_atbranch_type-branch)
		* [bool tiq::tree::BranchNode<K,T>::has_branch_begin()](#bool-tiqtreebranchnodekthas_branch_begin)
		* [bool tiq::tree::BranchNode<K,T>::has_branch_end()](#bool-tiqtreebranchnodekthas_branch_end)
		* [bool tiq::tree::BranchNode<K,T>::is_branch_begin(branch_type branch)](#bool-tiqtreebranchnodektis_branch_beginbranch_type-branch)
		* [bool tiq::tree::BranchNode<K,T>::is_branch_end(branch_type branch)](#bool-tiqtreebranchnodektis_branch_endbranch_type-branch)
		* [bool tiq::tree::BranchNode<K,T>::has_branch(branch_type branch)](#bool-tiqtreebranchnodekthas_branchbranch_type-branch)
		* [bool tiq::tree::BranchNode<K,T>::has_data(bool include_erased=false)](#bool-tiqtreebranchnodekthas_databool-include_erasedfalse)
		* [bool tiq::tree::BranchNode<K,T>::has_data(branch_type branch, bool include_erased=false)](#bool-tiqtreebranchnodekthas_databranch_type-branch-bool-include_erasedfalse)
		* [const branch_type* tiq::tree::BranchNode<K,T>::branch_begin()](#const-branch_type-tiqtreebranchnodektbranch_begin)
		* [const branch_type* tiq::tree::BranchNode<K,T>::branch_end()](#const-branch_type-tiqtreebranchnodektbranch_end)
		* [size_t tiq::tree::BranchNode<K,T>::size()](#size_t-tiqtreebranchnodektsize)
		* [BranchVector tiq::tree::BranchNode<K,T>::keys()](#branchvector-tiqtreebranchnodektkeys)
	* [class tiq::tree::BranchTree<N,A>](#class-tiqtreebranchtreena)
		* [node_ptr_t tiq::tree::BranchTree<N,A>::insert(node_ptr_t node, value_type data, branch_type branch)](#node_ptr_t-tiqtreebranchtreenainsertnode_ptr_t-node-value_type-data-branch_type-branch)
		* [node_ptr_t tiq::tree::BranchTree<N,A>::remove(node_ptr_t node, branch_type branch)](#node_ptr_t-tiqtreebranchtreenaremovenode_ptr_t-node-branch_type-branch)
		* [node_ptr_t tiq::tree::BranchTree<N,A>::remove(node_ptr_t node)](#node_ptr_t-tiqtreebranchtreenaremovenode_ptr_t-node)
		* [node_ptr_t tiq::tree::BranchTree<N,A>::erase(node_ptr_t node, branch_type branch, bool remove = false)](#node_ptr_t-tiqtreebranchtreenaerasenode_ptr_t-node-branch_type-branch-bool-remove--false)
		* [node_ptr_t tiq::tree::BranchTree<N,A>::erase(node_ptr_t node)](#node_ptr_t-tiqtreebranchtreenaerasenode_ptr_t-node)
		* [node_ptr_t tiq::tree::BranchTree<N,A>::find_min(node_ptr_t node, branch_type branch, bool search_erased=false)](#node_ptr_t-tiqtreebranchtreenafind_minnode_ptr_t-node-branch_type-branch-bool-search_erasedfalse)
		* [node_ptr_t tiq::tree::BranchTree<N,A>::find_min(branch_type branch, bool search_erased=false)](#node_ptr_t-tiqtreebranchtreenafind_minbranch_type-branch-bool-search_erasedfalse)
		* [node_ptr_t tiq::tree::BranchTree<N,A>::find_min(node_ptr_t node, bool search_erased=false)](#node_ptr_t-tiqtreebranchtreenafind_minnode_ptr_t-node-bool-search_erasedfalse)
		* [node_ptr_t tiq::tree::BranchTree<N,A>::find_min(bool search_erased)](#node_ptr_t-tiqtreebranchtreenafind_minbool-search_erased)
		* [node_ptr_t tiq::tree::BranchTree<N,A>::find_max(node_ptr_t node, branch_type branch, bool search_erased=false)](#node_ptr_t-tiqtreebranchtreenafind_maxnode_ptr_t-node-branch_type-branch-bool-search_erasedfalse)
		* [node_ptr_t tiq::tree::BranchTree<N,A>::find_max(branch_type branch, bool search_erased=false)](#node_ptr_t-tiqtreebranchtreenafind_maxbranch_type-branch-bool-search_erasedfalse)
		* [node_ptr_t tiq::tree::BranchTree<N,A>::find_max(node_ptr_t node, bool search_erased=false)](#node_ptr_t-tiqtreebranchtreenafind_maxnode_ptr_t-node-bool-search_erasedfalse)
		* [node_ptr_t tiq::tree::BranchTree<N,A>::find_max(bool search_erased=false)](#node_ptr_t-tiqtreebranchtreenafind_maxbool-search_erasedfalse)
		* [node_ptr_t tiq::tree::BranchTree<N,A>::find_next(node_ptr_t node, branch_type branch, bool search_erased=false)](#node_ptr_t-tiqtreebranchtreenafind_nextnode_ptr_t-node-branch_type-branch-bool-search_erasedfalse)
		* [node_ptr_t tiq::tree::BranchTree<N,A>::find_next(node_ptr_t node, bool search_erased=false)](#node_ptr_t-tiqtreebranchtreenafind_nextnode_ptr_t-node-bool-search_erasedfalse)
		* [node_ptr_t tiq::tree::BranchTree<N,A>::find_prev(node_ptr_t node, branch_type branch, bool search_erased=false)](#node_ptr_t-tiqtreebranchtreenafind_prevnode_ptr_t-node-branch_type-branch-bool-search_erasedfalse)
		* [node_ptr_t tiq::tree::BranchTree<N,A>::find_prev(node_ptr_t node, bool search_erased=false)](#node_ptr_t-tiqtreebranchtreenafind_prevnode_ptr_t-node-bool-search_erasedfalse)
		* [node_ptr_t tiq::tree::BranchTree<N,A>::find_nth(node_ptr_t node, size_t index, branch_type branch, bool search_erased=false)](#node_ptr_t-tiqtreebranchtreenafind_nthnode_ptr_t-node-size_t-index-branch_type-branch-bool-search_erasedfalse)
		* [node_ptr_t tiq::tree::BranchTree<N,A>::find_nth(size_t index, branch_type branch, bool search_erased=false)](#node_ptr_t-tiqtreebranchtreenafind_nthsize_t-index-branch_type-branch-bool-search_erasedfalse)
		* [node_ptr_t tiq::tree::BranchTree<N,A>::find_nth(node_ptr_t node, size_t index, bool search_erased=false)](#node_ptr_t-tiqtreebranchtreenafind_nthnode_ptr_t-node-size_t-index-bool-search_erasedfalse)
		* [node_ptr_t tiq::tree::BranchTree<N,A>::find_nth(size_t index, bool search_erased=false)](#node_ptr_t-tiqtreebranchtreenafind_nthsize_t-index-bool-search_erasedfalse)
		* [size_t tiq::tree::BranchTree<N,A>::find_index(node_ptr_t node, node_ptr_t parent, branch_type branch, bool search_erased=false)](#size_t-tiqtreebranchtreenafind_indexnode_ptr_t-node-node_ptr_t-parent-branch_type-branch-bool-search_erasedfalse)
		* [size_t tiq::tree::BranchTree<N,A>::find_index(node_ptr_t node, branch_type branch, bool search_erased=false)](#size_t-tiqtreebranchtreenafind_indexnode_ptr_t-node-branch_type-branch-bool-search_erasedfalse)
		* [size_t tiq::tree::BranchTree<N,A>::find_index(node_ptr_t node, node_ptr_t parent, bool search_erased=false)](#size_t-tiqtreebranchtreenafind_indexnode_ptr_t-node-node_ptr_t-parent-bool-search_erasedfalse)
		* [size_t tiq::tree::BranchTree<N,A>::find_index(node_ptr_t node, bool search_erased=false)](#size_t-tiqtreebranchtreenafind_indexnode_ptr_t-node-bool-search_erasedfalse)
		* [size_t tiq::tree::BranchTree<N,A>::size(branch_type branch, bool search_erased=false)](#size_t-tiqtreebranchtreenasizebranch_type-branch-bool-search_erasedfalse)
		* [size_t tiq::tree::BranchTree<N,A>::size(bool search_erased=false)](#size_t-tiqtreebranchtreenasizebool-search_erasedfalse)
* [Tests](#tests)
* [Additional Information](#additional-information)
	* [Commands](#commands)
		* [all (default)](#all-default)
		* [custom](#custom)
		* [lib](#lib)
		* [clear](#clear)
* [License](#license)

## Dependencies

No external dependencies.

## Build

Tested using GNU G++ compiler with flag -std=c++17. So it is recommended to use C++ 17 or higher version of compiler. Compiling with another compilers might need code corrections.

`make` default command builds the tests and creates **test.exe** file in the root directory.

`make custom` builds the playground file location `test/mtest.cpp`, and creates **mtest.exe** file in the root directory of the library.

## Documentation

### class tiq::tree::Node\<T\>

*(declared in tq_tree.h)*

Base tree structure to store data with type **T**. It has default *constructor* and virtual *destructor*.

It defines **value_type** type that equals **T**. All node types *must* declare **value_type** type in order to be used as tree nodes.

#### const T& tiq::tree::Node\<T\>::data()

Returns a const reference to the stored data. If no data is stored in the node *(it is an end node)*, throws **std::bad_optional_access**.

#### bool tiq::tree::Node\<T\>::is_end()

Return **true** if the node is an **end node**.

>[!NOTE]
>End node is a node that doesn't hold any data, nor is it supposed to be accessed via **data()** or any other functions.

___

### class tiq::tree::Tree\<N,A\>

*(declared in tq_tree.h)*

Accepts **2** template parameters.

* **N** is a node class that is used by the tree to hold data.
* **A** is an allocator class that is used to allocate memory for nodes. By default **std::allocator\<N\>** is used.

Standard *red-black* binary tree implementation, that provides very flexible **inserting/erasing** and **searching** functions, that allow you to navigate over the actual tree nodes. It has default *constructor* and virtual *destructor*.

>[!NOTE]
>Copy constructor and copy assignment operator are deleted for this class.

Defines the next public types:
* **node_ptr_t = N\***
* **comparator_fn_t = std::function<int(const T&)>**

#### node_ptr_t tiq::tree::Tree\<N,T\>::root()

Returns the root node of the tree.

#### node_ptr_t tiq::tree::Tree\<N,T\>::begin()

Returns the first *(left most)* node in the tree that holds data, or returns the end node, if the tree is empty.

#### node_ptr_t tiq::tree::Tree\<N,T\>::end()

Returns the last **end node** in the tree.

#### node_ptr_t tiq::tree::Tree\<N,T\>::find(comparator_fn_t comp)

Accepts one parameter **comp** - comparison function, and scans the tree starting from the **root** node.

The comparison function is being called for node that is being scanned, and if the function returns **0**, current node is returned from the **find** function, otherwise if the function returns **1** - the right node of the current node is selected for consecutive scan, and if the **-1** is returned, the left node of the current node is selected for the consecutive scan. If at any point an **end node** is chosen for scanning, the node is returned immediately.

***Example:***

```c++
tiq::tree::Tree<tiq::tree::Node<int>> t;

// some items are inserted into the tree.
...

//        [3]
//      /     \
//    [2]     [4]
//   /   \   /   \
// [e1] [e2][e3] [5]
//              /   \
//            [e4]  [e5]

// Binary search comparison function
int find_me = 5;
auto node = t->find([find_me](node_ptr_t node){
        if (node->data() == find_me) return 0;
        if (node->data() > find_me) return -1;
        return 1;
});

std::cout << node->data() << std::endl; // 5
```

#### node_ptr_t tiq::tree::Tree\<N,T\>::find(node_ptr_t node, comparator_fn_t comp)

The same as above, with the only difference that you are allowed to pass the starting node you want to perform scan for as a first argument of the function.

#### node_ptr_t tiq::tree::Tree\<N,T\>::find_min(node_ptr_t node = nullptr)

Accepts one parameter - **node** (by default is **nullptr**), and returns the left most child of this node that holds data. If a node is not passed, **root** node is used instead. If end node is passed, it is returned from the function immediately.

***Example:***

```c++
// Given tree:
//            [6]
//         /       \
//      [2]         [9] <--n1
//    /     \     /     \
//  [e1]   [e2] [8]     [10] <--n2
//             /   \    /  \
//           [7]  [e3][e4] [e5] <--n3
//          /   \
//        [e6]  [e7]
tree->find_min(n1)->data(); // 7
tree->find_min(n2)->data(); // 10 (the same node).
tree->find_min(n3); // end node (e5)
```

#### node_ptr_t tiq::tree::Tree\<N,T\>::find_max(node_ptr_t node = nullptr)

Accepts one parameter - **node** (by default is **nullptr**), and returns the right most child of this node that holds data. If a node is not passed, **root** node is used instead. If end node is passed, it is returned from the function immediately.

#### node_ptr_t tiq::tree::Tree\<N,T\>::find_next(node_ptr_t node)

Accepts one parameter **node**, and returns the next node in the tree that holds the data. If no node was found, it returns **end node**. If end node was passed, it will try to find the next node anyway.

***Example:***

```c++
// Given tree:
//            [6] <--n1
//         /       \
//      [2] <--n2   [9]
//    /     \     /     \
//  [e1]    [e2][8]     [10]
//             /   \    /  \
//           [7]  [e3][e4] [e5] <--n4
//          /   \
//  n3--> [e6]  [e7]

tree->find_next(n1)->data(); // 7
tree->find_next(n2)->data(); // 6
tree->find_next(n3)->data(); // 7
tree->find_next(n4); // end node (e5)
```

#### node_ptr_t tiq::tree::Tree\<N,T\>::find_prev(node_ptr_t node)

Accepts one parameter **node**, and returns the previous node in the tree that holds the data. If no node was found, it returns **end node**. If end node was passed, it will try to find the previous node anyway.

#### node_ptr_t tiq::tree::Tree\<N,T\>::parent(node_ptr_t node)

Accepts one parameter **node**, and returns the parent node of this node. Returns **nullptr** of the passed node is a **root** node.

***Example:***

```c++
// Given tree:
//            [6] <--n2
//         /       \
//      [2] <--n1   [9]
//     /   \       /   \
//   [e1] [e2]   [e3]  [e4]

tree->parent(n1)->data(); // 6
tree->parent(n2); // nullptr
```

#### node_ptr_t tiq::tree::Tree\<N,T\>::left(node_ptr_t node)

Accepts one parameter **node** and returns a direct left child of this node. If an **end node** was passed, it returns the passed node immediately.

***Example:***

```c++
// Given tree:
//            [6] <--n2
//         /       \
//      [2] <--n1   [9]
//     /   \       /   \
//   [e1] [e2]   [e3]  [e4] <--n3

tree->left(n1); // end node (e1)
tree->left(n2)->data(); // 2
tree->left(n3); // end node (e3)
```

#### node_ptr_t tiq::tree::Tree\<N,T\>::right(node_ptr_t node)

Accepts one parameter **node** and returns a direct right child of this node. If an **end node** was passed, it returns the passed node immediately.

#### node_ptr_t tiq::tree::Tree\<N,T\>::before(node_ptr_t node = nullptr)

Accepts one parameter **node** and returns an *end node* that goes directly before the current node. So if one inserts data into the node, it will be immediately before the passed node.

If no node is passed, the **root** node is used instead, meaning it will return the **end node** that is the left node of the first node in the tree that holds data.

***Example:***

```c++
// Given tree:
//            [6] <--n2
//         /       \
//      [2] <--n1   [9]
//     /   \       /   \
//   [e1] [e2]   [e3]  [e4] <--n3

tree->left(n1); // end node (e1)
tree->left(n2); // end node (e2)
tree->left(n3); // end node (e4)
```

#### node_ptr_t tiq::tree::Tree\<N,T\>::after(node_ptr_t node = nullptr)

Accepts one parameter **node**, and returns an *end node* that goes directly after the current node. So if one inserts data into the node, it will be immediately after the passed node.

If no node is passed, the **root** node is used instead, meaning it will return the **end node** that is the right node of the last node in the tree that holds data.

#### node_ptr_t tiq::tree::Tree\<N,T\>::insert(node_ptr_t node, T data)

Accepts 2 parameters: **node** - where insert the data, and **data** - the actual data that needs to be inserted.

It inserts data into the node, and returns the node.

If **end node** was passed, the node becomes a regular node, rebalances the tree if needed.

If a regular node was passed, it just updates the value of the node.

#### node_ptr_t tiq::tree::Tree\<N,T\>::erase(node_ptr_t node)

Accepts one parameter **node**, removes it from the tree, and destroys it.

If **end node** was passed, it throws **std::logic_error**.

After the node is removed, rebalancing the tree may happen.

It returns a node that is the children of the erased node, but not necessarily the direct.

#### void tiq::tree::Tree\<N,T\>::clear()

Removes all the elements from the tree.

#### size_t tiq::tree::size()

Returns the number of the elements in the tree.

___

### class tiq::tree::CountNode\<T\>

*(declared in tq_count_tree.h)*

Accepts one template parameter **T** - the type of data that needs to be stored in the tree.

Extends **tiq::tree::Node**.

Has inherited **T data()** and **bool is_end()** functions.

The class is used with **tiq::tree::CountTree** class.

#### size_t tiq::tree::CountNode\<T\>::count()

Returns the **number of children nodes** that hold data **+ 1** (current node).

End nodes return **0**.

___

### class tiq::tree::CountTree\<N,A\>

*(declared in tq_count_tree.h)*

Accepts **2** template parameters.

* **N** is a node class that is used by the tree to hold data.
* **A** is an allocator class that is used to allocate memory for nodes. By default **std::allocator\<N\>** is used.

Extends **tiq::tree::Tree\<N,A\>**.

The main feature of the **CountTree** is that it additionally keeps track for the node **indexes** in the tree, and provides some useful additional function to operate nodes and indexes.

#### node_ptr_t tiq::tree::CountTree\<N,A\>::find_nth(size_t count)

Accepts one parameter - index of the node in the tree. Returns the **node** that holds the value that has index = **count**, or **end node** if the index is out of the bound.

#### node_ptr_t tiq::tree::CountTree\<N,A\>::find_nth(node_ptr_t node, size_t count)

The same as above, but accepts an extra **node** parameter, and the search for the index will be done in scope of the passed node.

***Example:***
```c++
// Given CountTree:
//            [6] <--n1
//         /       \
//      [2] <--n2   [9] <--n3
//    /     \     /     \
//  [e1]    [e2][8]     [10]
//             /   \    /  \
//           [7]  [e3][e4] [e5] <--n4
//          /   \
//        [e6]  [e7]

tree->find_nth(4)->data(); // 9
tree->find_nth(n1, 3)->data() // 8
tree->find_nth(n2, 2); // end node (e2)
tree->find_nth(n2, 0)->data(); // 2
tree->find_next(n3, 3)->data(); // 9
tree->find_next(n4, 1); // end node (e5)
```

#### size_t tiq::tree::find_index(node_ptr_t node, node_ptr_t parent = nullptr)

Accepts 2 nodes - the **node** which index needs to be returned, and **parent** node - the scope the index needs to be provided for.

If no **parent** was passed, the **root** node is used instead.

if **node** is not a child of **parent** - the **std::logic_error** is thrown.

Returns **index** of the passed node in the tree (subtree).

___

### class tiq::tree::BranchNode\<K,T\>

*(declared in tq_branch_tree.h)*

Accepts two template parameter **T** - the type of data that needs to be stored, and **K** - is the type of the branch versions.

>[!IMPORTANT]
>The **branch** *(K)* template parameter must implement operators **>**, **<** and **==**.

The node is used as a node for **tiq::tree::BranchTree**.

>[!NOTE]
>Don't be confused with the **key**, there is no concept of keys in the **BranchTree**. **Branch** is a different concept that is described under the [**BranchTree**](#tiqtreebranchtreena) section.

It exposes **3** public types:

* **value_type** - this equals to the template parameter **T**.
* **branch_type** - this equals to the template parameter **K**.
* **BranchVector** - which is an alias for **std::vector\<branch_type\>**.

#### size_t tiq::tree::BranchNode<K,T>::count()

Returns the number of **children nodes** of the current node that hold data **+1** (current node).

#### size_t tiq::tree::BranchNode<K,T>::count(branch_type branch, bool include_erased=false)

Returns the number of **children nodes** of the current node that hold data in the **branch**, **+1** and if current node holds data in the **branch**. If the **include_erased** parameters is set to `true`, also adds the number of erased nodes.

***Example:***

```c++
tiq::tree::BranchTree tree<tiq::tree::BranchNode<string, int>>;
tree->insert(tree->end(), "foo", 1);
tree->insert(tree->end(), "bar", 2);
tree->insert(tree->end(), "baz", 3);
tree->erase(tree->end(), 3);

tree->root()->count(1); // 1
tree->root()->count(2); // 2
tree->root()->count(3); // 3
tree->root()->count(10); // 3
tree->root()->count(3,true); // 4
```

#### const value_type& tiq::tree::BranchNode<K,T>::data()

Returns **held** in the node data *in the highest branch*. If no data is held in the node, or the node was erased at any branch, **std::logic_error** is thrown.

#### const value_type& tiq::tree::BranchNode<K,T>::data(branch_type branch)

Returns **held** in the node data in the passed **branch**. If no data held in the **branch**, or the node is an **end node**, the **std::logic_error** is thrown.

***Example:***

```c++
tiq::tree::BranchTree tree<tiq::tree::BranchNode<string, int>>;
auto node = tree->insert(tree->end(), "foo", 1);
tree->insert(node, "bar", 2);
tree->insert(node, "baz", 3);

// Erase the node at branch 3
tree->erase(node, 3);

node->data(1) // foo
node->data(2) // bar
node->data(10) // throws std::logic_error as the node was erased at branch 3
node->data() // throws std::logic_error as the node was erased at branch 3
```

#### const value_type* tiq::tree::BranchNode<K,T>::data_at(branch_type branch)

Returns **held** in the node data *exactly at* the passed **branch**, or **nullptr** if there was no data inserted at the **branch**.

#### bool tiq::tree::BranchNode<K,T>::has_branch_begin()

Returns **true** if the node contains data at least in one branch.

>[!NOTE]
>If the node was erased before the lower branch that holds the data, the function returns false.

#### bool tiq::tree::BranchNode<K,T>::has_branch_end()

Returns **true** if the node was erased at least in one branch.

>[!NOTE]
>If the node was erased before the lower branch that holds the data, the function returns false.

***Example:***

```c++
tiq::tree::BranchTree tree<tiq::tree::BranchNode<string, int>>;

auto node1 = tree->insert(tree->end(), "foo", 1);
auto node2 = tree->insert(tree->end(), "bar", 2);
auto node3 = tree->insert(tree->end(), "baz", 3);

tree->erase(node1, 2);
tree->erase(node2, 0);

node1->has_branch_begin(); // true
node1->has_branch_end(); // true

node2->has_branch_begin(); // false
node2->has_branch_end(); // false

node3->has_branch_begin(); // true
node3->has_branch_end(); // false
```

#### bool tiq::tree::BranchNode<K,T>::is_branch_begin(branch_type branch)

Accepts one argument **branch**, and returns **true** if the nodes *lowest* branch that holds data equals to **branch**.

#### bool tiq::tree::BranchNode<K,T>::is_branch_end(branch_type branch)

Accepts one argument **branch**, and returns **true** if the nodes *lowest* branch that was erased equals to **branch**.

#### bool tiq::tree::BranchNode<K,T>::has_branch(branch_type branch)

Accepts one argument **branch**, and returns **true** if the node contains data exactly at **branch** or the node was erased *exactly* at **branch**.

***Example:***

```c++
tiq::tree::BranchTree tree<tiq::tree::BranchNode<string, int>>;

auto node = tree->insert(tree->end(), "foo", 1);
tree->erase(node, 2);
tree->insert(node, "bar", 3);
tree->insert(node, "baz", 4);

node->size(); // 4
node->has_branch(1); // true
node->has_branch(2); // true
node->has_branch(3); // true
node->has_branch(4); // true
node->has_branch(10); // false
```

#### bool tiq::tree::BranchNode<K,T>::has_data(bool include_erased=false)
If the **include_erased** parameter is `true`, then returns **true** if the node has data at least in one branch.
Otherwise if the **include_erased** is `false`_(default)_, returns **true** if there is at least one **insert** value, and no values **erased**.

#### bool tiq::tree::BranchNode<K,T>::has_data(branch_type branch, bool include_erased=false)

Accepts **branch** argument that corresponds to a branch you want to investigate, and the *bool* **include_erased** (default to `false`) parameter, and returns **true** if the node holds data in the **branch**, or in case of **include_erased** is `true`, it returns true if the node holds data or holds "erases".

#### const branch_type* tiq::tree::BranchNode<K,T>::branch_begin()

Returns **const pointer** to the *lowest* branch in the node that holds value, or **nullptr** if it does not exist.

#### const branch_type* tiq::tree::BranchNode<K,T>::branch_end()

Returns **const pointer** to the *lowest* branch in the node that was erased, or **nullptr** if it does not exist.

***Example:***

```c++
tiq::tree::BranchTree tree<tiq::tree::BranchNode<string, int>>;

auto node1 = tree->insert(tree->end(), "foo", 1);
auto node2 = tree->insert(tree->end(), "bar", 2);
tree->insert(node1, "baz", 2);
tree->erase(node1, 3);

*(node1->branch_begin); // 1
*(node1->branch_end); // 3
*(node2->branch_begin); // 2
node2->branch_end; // nullptr
```

#### size_t tiq::tree::BranchNode<K,T>::size()

Returns the number of branches that holds value + number of erased branches.

#### BranchVector tiq::tree::BranchNode<K,T>::keys()

Returns the list of branches that exist in the node *(that hold value or were erased)* in the increasing order.

***Example:***

```c++
tiq::tree::BranchTree tree<tiq::tree::BranchNode<string, int>>;

auto node = tree->insert(tree->end(), "foo", 1);
tree->erase(node, 2);
tree->insert(node, "bar", 3);
tree->insert(node, "baz", 4);

node->keys(); // {1,2,3,4}
```

___

### class tiq::tree::BranchTree\<N,A\>

*(declared in tq_branch_tree.h)*

Accepts **2** template parameters.

* **N** is a node class that is used by the tree to hold data.
* **A** is an allocator class that is used to allocate memory for nodes. By default **std::allocator\<N\>** is used.

Extends **tiq::tree::CountTree\<N,A\>**.

BranchTree allows you to have multiple versions (branches) of your tree in a single tree instance. The higher branch also contains all lower branches.

***Example:***

Given a tree with 5 values in the branch **1**, and 4 values in branch **2**:

```
1: 1, 2, 3, 4, 5
2:          7, 8, 9, 10
```

Then there are **5** values in the branch **1** (1,2,3,4,5), and **7** values in branch **2** (1,2,3,7,8,9,10).

It exposes next public types:

* **value_type** - equals to **N::value_type**.
* **node_ptr_t** - equals to **N**.
* **branch_type** - equals to **N::branch_type**.
* **comparator_fn_t** - equals to **std::function<int(const value_type&)>**.

#### node_ptr_t tiq::tree::BranchTree\<N,A\>::insert(node_ptr_t node, value_type data, branch_type branch)

Inserts **data** into **node** at **branch**. If the node is an **end node**, it becomes a regular node, and tree rebalancing might occur afterwards.

If the **node** is a regular node, the data is inserted (or updated) in the node, without changing the internal tree structure.

#### node_ptr_t tiq::tree::BranchTree\<N,A\>::remove(node_ptr_t node, branch_type branch)

Removes the branch **data** or **erase point** from the node. If the node becomes empty, the node is removed.

>[!NOTE]
>Don't be confused with **erase** which erases the node on a specific branch, without removing the node, but rather adding corresponding **erase point** to the node.

#### node_ptr_t tiq::tree::BranchTree\<N,A\>::remove(node_ptr_t node)

Removes the whole node altogether. Tree rebalancing might occur.

#### node_ptr_t tiq::tree::BranchTree\<N,A\>::erase(node_ptr_t node, branch_type branch, bool remove = false)

If **remove** is **true**, then it works exactly as **tiq::tree::BranchTree\<N,A\>::remove(node_ptr_t, branch_type)**, otherwise it does not remove the node, bat erases *(adds corresponding erase point)* to the specific **branch** in the **node**.

***Example:***

```c++
tiq::tree::BranchTree tree<tiq::tree::BranchNode<string, int>>;

auto node = tree->insert(tree->end(), "foo", 1);
tree->insert(node, "bar", 2);
tree->insert(node, "baz", 4);
tree->erase(node, 3);

node->data(); // bar

// Removes erase point
tree->remove(node, 3);

node->data(); // baz
```

#### node_ptr_t tiq::tree::BranchTree\<N,A\>::erase(node_ptr_t node)

Works exactly as **tiq::tree::BranchTree\<N,A\>::remove(node_ptr_t)**.

#### node_ptr_t tiq::tree::BranchTree\<N,A\>::find_min(node_ptr_t node, branch_type branch, bool search_erased=false)

Returns the **left most** child of the passed **node** in the passed **branch**. If no corresponding node was found, returns an **end node**. The **search_erased** parameter allows it to search over erased nodes as well.

***Example:***

```c++
// Given a tree (branch is reflected in parentheses):
//             [5(1)]
//           /        \
//        [3(2)]     [6(2)]
//       /     \     /     \
//    [2(1)]   [e1][e2]    [e3]

tree->find_min(tree->root(), 2)->data(); // 3
```

#### node_ptr_t tiq::tree::BranchTree\<N,A\>::find_min(branch_type branch, bool search_erased=false)

Same as above, with the difference that the scan is performed on a **root** node. The **search_erased** parameter allows it to search over erased nodes as well.

#### node_ptr_t tiq::tree::BranchTree\<N,A\>::find_min(node_ptr_t node, bool search_erased=false)

Same as [find_min(node_ptr_t node, branch_type branch, bool search_erased)](#node_ptr_t-tiqtreebranchtreenafind_minnode_ptr_t-node-branch_type-branch-bool-search_erasedfalse) but with the difference that **branch** is the maximum branch among the tree.

#### node_ptr_t tiq::tree::BranchTree\<N,A\>::find_min(bool search_erased)

Same as above, with the difference that the scan is performed on a **root** node. The **search_erased** parameter allows it to search over erased nodes as well.

#### node_ptr_t tiq::tree::BranchTree\<N,A\>::find_max(node_ptr_t node, branch_type branch, bool search_erased=false)

Returns the **right most** child of the passed **node** in the passed **branch**. If no corresponding node was found, returns an **end node**. The **search_erased** parameter allows it to search over erased nodes as well.

#### node_ptr_t tiq::tree::BranchTree\<N,A\>::find_max(branch_type branch, bool search_erased=false)

Same as above, with the difference that the scan is performed on a **root** node. The **search_erased** parameter allows it to search over erased nodes as well.

#### node_ptr_t tiq::tree::BranchTree\<N,A\>::find_max(node_ptr_t node, bool search_erased=false)

Same as [find_max(node_ptr_t node, branch_type branch, bool search_erased)](#node_ptr_t-tiqtreebranchtreenafind_maxnode_ptr_t-node-branch_type-branch-bool-search_erasedfalse) but with the difference that **branch** is the maximum branch among the tree.

***Example:***

```c++
// Given a tree (branch is reflected in parentheses):
//             [5(1)]
//           /        \
//        [3(2)]     [6(2)]
//       /     \     /     \
//    [2(1)]   [e1][e2]    [e3]

tree->find_max(tree->root())->data(); // 6
```

#### node_ptr_t tiq::tree::BranchTree\<N,A\>::find_max(bool search_erased=false)

Same as above, with the difference that the scan is performed on a **root** node. The **search_erased** parameter allows it to search over erased nodes as well.

#### node_ptr_t tiq::tree::BranchTree\<N,A\>::find_next(node_ptr_t node, branch_type branch, bool search_erased=false)

Returns the node that is next to the passed **node**, in the **branch**, or **end node** if the node was not found. The **search_erased** parameter allows it to search over erased nodes as well.

***Example:***

```c++
// Given a tree (branch is reflected in parentheses):
//             [5(2)] <--n1
//           /        \
//        [3(1)] <--n2 [6(1)]
//       /     \     /     \
//    [2(2)]   [e1][e2]    [e3]

tree->find_next(n1, 1)->data(); // 6
tree->find_next(n1, 2)->data(); // 6
tree->find_next(n2, 1)->data(); // 6
tree->find_next(n2, 2)->data(); // 5
```

#### node_ptr_t tiq::tree::BranchTree\<N,A\>::find_next(node_ptr_t node, bool search_erased=false)

Same as above, but the search is performed on the **highest** branch in the tree. The **search_erased** parameter allows it to search over erased nodes as well.

#### node_ptr_t tiq::tree::BranchTree\<N,A\>::find_prev(node_ptr_t node, branch_type branch, bool search_erased=false)

Returns the node that is previous to the passed **node**, in the **branch**, or **end node** if the node was not found. The **search_erased** parameter allows it to search over erased nodes as well.

#### node_ptr_t tiq::tree::BranchTree\<N,A\>::find_prev(node_ptr_t node, bool search_erased=false)

Same as above, but the search is performed on the **highest** branch in the tree. The **search_erased** parameter allows it to search over erased nodes as well.

#### node_ptr_t tiq::tree::BranchTree\<N,A\>::find_nth(node_ptr_t node, size_t index, branch_type branch, bool search_erased=false)

Finds a child node in the given **node** and **branch** which index is equal to the passed **index**. If no node was found, returns **end node**. The **search_erased** parameter allows it to search over erased nodes as well.

#### node_ptr_t tiq::tree::BranchTree\<N,A\>::find_nth(size_t index, branch_type branch, bool search_erased=false)

Same as above, but the scan is performed on the **root** node. The **search_erased** parameter allows it to search over erased nodes as well.

#### node_ptr_t tiq::tree::BranchTree\<N,A\>::find_nth(node_ptr_t node, size_t index, bool search_erased=false)

Finds a child node in the given **node** in a **highest** branch in the tree, which index is equal to the passed **index**. If no node was found, returns **end node**. The **search_erased** parameter allows it to search over erased nodes as well.

#### node_ptr_t tiq::tree::BranchTree\<N,A\>::find_nth(size_t index, bool search_erased=false)

Same as above, but the scan is performed on the **root** node. The **search_erased** parameter allows it to search over erased nodes as well.

#### size_t tiq::tree::BranchTree\<N,A\>::find_index(node_ptr_t node, node_ptr_t parent, branch_type branch, bool search_erased=false)

Returns an index of the passed **node** in the **branch** within the **parent** node. The **search_erased** parameter allows it to search over erased nodes as well.

#### size_t tiq::tree::BranchTree\<N,A\>::find_index(node_ptr_t node, branch_type branch, bool search_erased=false)

Returns an index of the passed **node** in the **branch** within the **root** node. The **search_erased** parameter allows it to search over erased nodes as well.

#### size_t tiq::tree::BranchTree\<N,A\>::find_index(node_ptr_t node, node_ptr_t parent, bool search_erased=false)

Returns an index of the passed **node** in the **highest** branch in the tree, within the **parent** node. The **search_erased** parameter allows it to search over erased nodes as well.

#### size_t tiq::tree::BranchTree\<N,A\>::find_index(node_ptr_t node, bool search_erased=false)

Same as above, but the scan is performed on the **root** node. The **search_erased** parameter allows it to search over erased nodes as well.

#### size_t tiq::tree::BranchTree\<N,A\>::size(branch_type branch, bool search_erased=false)

Returns the size of the tree in the given **branch**. The **search_erased** parameter allows it to search over erased nodes as well.

#### size_t tiq::tree::BranchTree\<N,A\>::size(bool search_erased=false)

Returns the size of the tree in the **highest** branch. The **search_erased** parameter allows it to search over erased nodes as well.

## Tests

Tests are defined under the **test** folder that leaves in the root folder of the library.

By running `make all` *(default command)*, the **test/test.cpp** file is being compiled and the **test.exe** file is being generated in the root directory of the library. Run the file, to run library tests.

## Additional Information

**makefile** contains `custom` command, that compiles **test/mtest.cpp** file. This file can be used as a quick start sandbox file, to try things out. There are some neat example already provided in the file.

### Commands

#### all (default)

Builds unit test and creates a **test.exe** executable file in a library root diretory.

#### custom

Builds a sandbox *test/mtest.cpp* file, and creates a **mtest.exe** executable file in a library root directory.

#### lib

Builds library file and generate an archive **liboutput.a** file that can be linked to the final executable file.

#### clear

Cleanup objects, executable and archive files.

## License

MIT

Have fun :)
