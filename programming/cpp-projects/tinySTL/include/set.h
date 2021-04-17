/*********************************************************************************************/
// class set:
// using rbtree as base container
// including some basic functions of set:
//
// 1. iterator begin() const;
// 2. iterator end() const;
// 3. size_type size() const;
// 4. bool empty() const;
// 5. iterator find(const T& v) const;
// 6. void insert(const T& v);
// 7. void erase(iterator pos);
// 8. void clear();

#ifndef __SET_H__
#define __SET_H__

#include "functional.h"
#include "memory.h"
#include "rbtree.h"

namespace tinySTL 
{
    template<typename T, typename Compare = less<T>, typename Alloc = allocator<__rbtree_node<T>>>
    class set {
    public:
            using size_type = unsigned int;
            using value_type = T;
            using iterator = __rbtree_iterator<T>;
            using const_iterator = const iterator;
            using reference = value_type&;
            using const_reference = const value_type&;
            using difference_type = int;
    protected:
            rbtree<T, Compare, Alloc> base_rbt;
    public:
            iterator begin() const {
                return base_rbt.begin();
            }
            iterator end() const {
                return base_rbt.end();
            }
            size_type size() const {
                return base_rbt.size();
            }
            bool empty() const {
                return base_rbt.empty();
            }
            // find a value
            iterator find(const T& v) const {
                return base_rbt.find(v);
            }
            // insert a value
            void insert(const T& v) {
                base_rbt.insert(v);
            }
            // erase an iterator
            void erase(iterator pos) {
                base_rbt.erase(pos);
            }
            void clear() {
                base_rbt.clear();
            }
    };
}

#endif