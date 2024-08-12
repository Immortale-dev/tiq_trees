# TIQ Trees

The repository provides C++ implementation of some useful and unique tree data structures.

## Table of Contest

TODO

## Dependencies

No dependencies

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

Returns a const reference to the stored data.

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
tiq::tree::Tree t;

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

### tiq::tree::BranchNode\<K,T\>

*(declared in tq_branch_tree.h)*

TODO: describe

___

### tiq::tree::BranchTree\<N,A\>

*(declared in tq_branch_tree.h)*

TODO: describe

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
