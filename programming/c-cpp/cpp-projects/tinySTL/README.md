# tinySTL

The tinySTL is a tiny STL writen by C++11/14/17 and generic programming, providing six basic components: containers,  iterators, adapters, algorithms, allocators and functions. It also contains three smart pointers(shared_ptr, unique_ptr, weak_ptr) and test codes.

## Test

> My test environment: Apple LLVM version 10.0.0 (clang-1000.10.44.4)

 1. git clone https://github.com/ifding/cpp-projects.git
 2. cd cpp-projects/tinySTL
 3. make test
 4. ./test
 5. make clean


## Compotents

### 1. test

**correctness**:
random data sets are generate to test the correctness of all the components above repeatedly.

### 2. smart pointers

Providing shared_ptr, unique_ptr and weak_ptr.

### 3. allocators

Providing a simple allocator. Users can customize their own allocators.

### 4. iterators

Using type_traits tricks

### 5. functions

Providing functions including less, greater, etc.

Providing a universal hash function for unordered_map.

### 6. containers

**vector**: dynamic array
 
**list**: bidirectional list

**set**: red-black tree

**map**: red-black tree

**unordered_map**: hash table
> begin(), end(), empty(), size(), push_back(), pop_back(), find(), insert(), erase(), clear(), copy constructor, move constructor, copy assignment operator, move assignment operator, destructor, etc

### 7. adapters

**stack**: using list as base container

**queue**: using list as base container

**priority_queue**: using vector as base container
> empty(), size(), push(), pop(), top(), front(), back()

### 8. algorithms

**sort**: using insertion sort/quick sort/heap sort

**stable_sort**: stable version, using insertion sort/merge sort

**efficiency**:
tinySTL shows some progress in efficiency compared to PJ STL and SGI STL in some aspects. For example, tinySTL is slightly better than PJ STL in set/map inserting, finding, erasing and in sort function. It also excels SGI STL in  set/map inserting, finding, erasing.


## Reference
- <https://github.com/cjc12/ezSTL>
- <https://github.com/Alinshans/MyTinySTL>
- <https://github.com/syn1w/TinySTL>