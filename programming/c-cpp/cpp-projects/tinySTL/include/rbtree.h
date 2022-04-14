/*********************************************************************************************/
// class rbtree:
// an internal class, basic data structure of set and map
//
// 1. iterator begin() const;
// 2. iterator end() const;
// 3. size_type size() const;
// 4. bool empty() const;
// 5. iterator find(const T& v) const;
// 6. void insert(const T& v);
// 7. void erase(iterator pos);
// 8. void clear();
// only for debug:
// 9. bool is_rbtree();

#ifndef __RBTREE_H__
#define __RBTREE_H__

#include "functional.h"
#include "iterator.h"
#include "memory.h"
#include "utility.h"

namespace tinySTL
{
    // color type of RB Tree
    using __rbtree_color_type = bool;
    const __rbtree_color_type __rbtree_red = false;
    const __rbtree_color_type __rbtree_black = true;

    // node of RB Tree
    template<typename T>
    class __rbtree_node {
    public:
            __rbtree_node(const T& t) : 
            color(__rbtree_red), parent(nullptr), left(nullptr), right(nullptr), data(t){ }
            // some type definition
            using color_type = __rbtree_color_type;
            using value_type = T;
            using pointer = __rbtree_node<value_type>*;

            // data members
            color_type color;
            pointer parent;
            pointer left;
            pointer right;
            value_type data;

            // find the minimum value
            static pointer minimum(pointer x) {
                while(x->left != nullptr)
                    x = x->left;
                return x;
            }
            // find the maximum value
            static pointer maximum(pointer x) {
                while(x->right != nullptr)
                    x = x->right;
                return x;
            }
    };

    // iterator of RB Tree
    template<typename T>
    class __rbtree_iterator : public iterator<bidirectional_iterator_tag, T> {
    public:
            using iterator = __rbtree_iterator<T>;
            using self = __rbtree_iterator<T>;
            using link_type = __rbtree_node<T>*;
            using pointer = T*;
            using reference = T&;
            using difference_type = int;

            // __rbtree_node pointer
            link_type node;

            __rbtree_iterator() = default;
            __rbtree_iterator(link_type x) : node(x) {}
            __rbtree_iterator(const iterator& x) : node(x.node) {}
            self& operator= (const iterator& x) {
                if( this != &x)
                    node = x.node;
                return *this;
            }

            bool operator==(const self& x) const {
                return node == x.node;
            }
            bool operator!=(const self& x) const {
                return node != x.node;
            }
            reference operator*() const {
                return (*node).data;
            }
            pointer operator->() const {
                return &(operator*());
            }
            self& operator++() {
                // if it has right node
                if(node->right != nullptr) {
                    node = node->right;
                    while(node->left != nullptr)
                        node = node->left;
                } else {
                    link_type parent_node = node->parent;
                    while(node == parent_node->right) {
                        node = parent_node;
                        parent_node = parent_node->parent;
                    }
                    if(node->right != parent_node)
                        node = parent_node;
                }
                return *this;
            }
            self operator++(int) {
                self tmp = *this;
                ++(*this);
                return tmp;
            }
            self& operator--() {
                // present node is header
                if(node->color == __rbtree_red && node->parent->parent) {
                    node = node->right;
                // if node has left child
                } else if(node->left != nullptr) {
                    node = node->left;
                    while(node->right != nullptr)
                        node = node->right;
                } else {
                    link_type parent_node = node->parent;
                    while(node == parent_node->left) {
                        node = parent_node;
                        parent_node = parent_node->parent;
                    }
                    node = parent_node;
                }
                return *this;
            }
            self operator--(int) {
                self tmp = *this;
                --(*this);
                return tmp;
            }
            self operator+(difference_type n) const {
                self tmp = *this;
                advance(tmp, n);
                return tmp;
            }
            self operator-(difference_type n) const {
                self tmp = *this;
                advance(tmp, -n);
                return tmp;
            }
    };

    // RB Tree class
    template<typename T, typename Compare = less<T>, typename Alloc = allocator<__rbtree_node<T>>>
    class rbtree {
    public:
            // some type definition for rbtree
            using rbtree_node = __rbtree_node<T>;
            using link_type = __rbtree_node<T>*;
            using size_type = unsigned int;
            using value_type = T;
            using iterator = __rbtree_iterator<T>;
            using const_iterator = const iterator;
            using reference = value_type&;
            using const_reference = const T&;
            using difference_type = int;

    protected:
            // data members of rbtree
            size_type _size;
            iterator header;
            Compare cmp;
    
    public:
            // constructor
            rbtree() : _size(0) {
                header.node = Alloc().allocate();
                header.node->color = __rbtree_red;
                header.node->parent = nullptr;
                header.node->left = header.node;
                header.node->right = header.node;
            }
            rbtree(const rbtree<T, Compare, Alloc>&) { }
            rbtree(rbtree<T, Compare, Alloc>&&) noexcept { }
            rbtree<T, Compare, Alloc>& operator=(const rbtree<T, Compare, Alloc>&) { }
            rbtree<T, Compare, Alloc>& operator=(rbtree<T, Compare, Alloc>&&) noexcept { }
            ~rbtree() {
                clear();
            }

    protected:
            // some common functions for header
            link_type root() const {
                return header.node->parent;
            }
            link_type leftmost() const {
                return header.node->left;
            }
            link_type rightmost() const {
                return header.node->right;
            }
            // rotate
            void left_rotate(link_type z);
            void right_rotate(link_type z);
            // fixup after inserting a new value
            void insert_fixup(link_type z);
            // fixup after deleting a value
            void delete_fixup(link_type parent, link_type child);
            void dfs_clear(link_type root);
            bool dfs_is_rbtree(link_type root, size_type count_of_black_now, 
                const size_type count_of_black);

    public:
            // some outer interfaces
            iterator begin() const {
                return leftmost();
            }
            iterator end() const {
                return header;
            }
            bool empty() const {
                return _size == 0;
            }
            size_type size() const {
                return _size;
            }
            iterator find(const value_type& v) const;
            void insert(const value_type& v);
            void erase(iterator pos);
            void clear();

            // interface for debug, check if the present tree is a rbtree
            bool is_rbtree();
    };

    // find a value, O(logN)
    template<typename T, typename Compare, typename Alloc>
    typename rbtree<T, Compare, Alloc>::iterator
        rbtree<T, Compare, Alloc>::find(const value_type& v) const {
            link_type curr = root();
            while(curr != nullptr) {
                if(cmp(v, curr->data)) {
                    curr = curr->left;
                } else if (cmp(curr->data, v)) {
                    curr = curr->right;
                } else {
                    return iterator(curr);
                }
            }
            return end();
    }

    // insert a value
    template<typename T, typename Compare, typename Alloc>
    void rbtree<T, Compare, Alloc>::insert(const value_type& v) {
        // node to be inserted
        link_type new_node = Alloc().allocate();
        Alloc().construct(new_node, v);
        // parent node of inserted node
        link_type parent_node = nullptr;
        // node used to find proper position
        link_type curr = root();
        // find the proper position to insert
        while(curr != nullptr) {
            parent_node = curr;
            if(cmp(v, curr->data)) {
                curr = curr->left;
            } else if(cmp(curr->data, v)) {
                curr = curr->right;
            } else {
                curr->data = v;
                return;
            }
        }
        new_node->parent = parent_node;
        // if it is an empty tree, curr = nullptr
        if(parent_node == nullptr) {
            header.node->parent = new_node;
            header.node->left = new_node;
            header.node->right = new_node;
            new_node->parent = header.node;
        } else if(cmp(v, parent_node->data)) {
            parent_node->left = new_node;
        } else {
            parent_node->right = new_node;
        }
        // fix up
        insert_fixup(new_node);
        // adjust header: left and right
        if(empty() || cmp(v, leftmost()->data)) {
            header.node->left = new_node;
        }
        if(empty() || cmp(rightmost()->data, v)) {
            header.node->right = new_node;
        }
        ++_size;
    }

    // erase
    template<typename T, typename Compare, typename Alloc>
    void rbtree<T, Compare, Alloc>::erase(iterator pos) {
        // adjust leftmost and rightmost
        if(pos == begin()) {
            header.node->left = (pos + 1).node;
        }
        if(pos == (end() - 1)) {
            header.node->right = (pos - 1).node;
        }
        link_type to_delete = nullptr;
        if(pos.node->left == nullptr || pos.node->right == nullptr) {
            to_delete = pos.node;
        } else {
            to_delete = (pos + 1).node;
        }
        // child: child node of to_delete node
        // parent: parent node of child
        link_type child = nullptr, parent = nullptr;
        if(to_delete->left != nullptr) {
            child = to_delete->left;
        } else {
            child = to_delete->right;
        }
        if(child != nullptr) {
            child->parent = to_delete->parent;
        }
        parent = to_delete->parent;
        if(to_delete == parent->left) {
            parent->left = child;
        } else if(to_delete == parent->right) {
            parent->right = child;
        }
        // copy data from node to be deleted to pos
        if(to_delete != pos.node) {
            pos.node->data = to_delete->data;
        }
        // if the deleted node is black, then fix up
        if(to_delete->color == __rbtree_black) {
            delete_fixup(parent, child);
        }
        // deallocate node
        Alloc().destroy(to_delete);
        Alloc().deallocate(to_delete);
        --_size;
    }

    template<typename T, typename Compare, typename Alloc>
    void rbtree<T, Compare, Alloc>::clear() {
        // dfs clear
        if(root() != nullptr) {
            dfs_clear(root());
        }
        header.node->left = header.node;
        header.node->right = header.node;
        header.node->parent = nullptr;
        header.node->color = __rbtree_red;
        _size = 0;
    }

    template<typename T, typename Compare, typename Alloc>
    void rbtree<T, Compare, Alloc>::left_rotate(link_type curr) {
        link_type right_node = curr->right;
        curr->right = right_node->left;
        if(right_node->left != nullptr) {
            right_node->left->parent = curr;
        }
        right_node->parent = curr->parent;
        // change root
        if(curr == root()) {
            header.node->parent = right_node;
        } else if(curr == curr->parent->left) {
            curr->parent->left = right_node;
        } else if(curr == curr->parent->right) {
            curr->parent->right = right_node;
        }
        right_node->left = curr;
        curr->parent = right_node;
    }

    template<typename T, typename Compare, typename Alloc>
    void rbtree<T, Compare, Alloc>::right_rotate(link_type curr) {
        link_type left_node = curr->left;
        curr->left = left_node->right;
        if(left_node->right != nullptr) {
            left_node->right->parent = curr;
        }
        left_node->parent = curr->parent;
        // change root
        if(curr == root()) {
            header.node->parent = left_node;
        } else if(curr == curr->parent->left) {
            curr->parent->left = left_node;
        } else if(curr == curr->parent->right) {
            curr->parent->right = left_node;
        }
        left_node->right = curr;
        curr->parent = left_node;
    }

    template<typename T, typename Compare, typename Alloc>
    void rbtree<T, Compare, Alloc>::insert_fixup(link_type now) {
        while(now != root() && now->parent->color == __rbtree_red) {
            if(now->parent == now->parent->parent->left) {
                link_type uncle = now->parent->parent->right;
                // if parent and uncle are both red, case 1
                if(uncle != nullptr && uncle->color == __rbtree_red) {
                    now->parent->color = __rbtree_black;
                    uncle->color = __rbtree_black;
                    uncle->color = __rbtree_black;
                    now->parent->parent->color = __rbtree_red;
                    now = now->parent->parent;
                } else { // if no uncle or uncle is __rbtree_black
                    // if now is a right child, case 2
                    if(now == now->parent->right) {
                        now = now->parent;
                        left_rotate(now);
                    }
                    // if now is a left chil, case 3
                    now->parent->color = __rbtree_black;
                    now->parent->parent->color = __rbtree_red;
                    right_rotate(now->parent->parent);
                }
            } else {
                link_type uncle = now->parent->parent->left;
                // if parent and uncle are both red
                if(uncle != nullptr && uncle->color == __rbtree_red) {
                    now->parent->color = __rbtree_black;
                    uncle->color = __rbtree_black;
                    now->parent->parent->color = __rbtree_red;
                    now = now->parent->parent;
                } else { // if no uncle or uncle is __rbtree_black
                    // if now is a left child
                    if(now == now->parent->left) {
                        now = now->parent;
                        right_rotate(now);
                    }
                    now->parent->color = __rbtree_black;
                    now->parent->parent->color = __rbtree_red;
                    left_rotate(now->parent->parent);
                }
            }
        }
        // root is always black
        root()->color = __rbtree_black;
    }

    template<typename T, typename Compare, typename Alloc>
    void rbtree<T, Compare, Alloc>::delete_fixup(link_type parent, link_type child) {
        while(child != root() && (!child || child->color == __rbtree_black)) {
            link_type sibling = nullptr;
            if(child == parent->left) {
                sibling = parent->right;
                // case 1: color of sibling is red
                if(sibling->color == __rbtree_red) {
                    sibling->color = __rbtree_black;
                    parent->color = __rbtree_red;
                    left_rotate(parent);
                    sibling = parent->right;
                }
                // case 2: color of sibling's children are both black
                if((!sibling->left || sibling->left->color == __rbtree_black) &&
                    (!sibling->right || sibling->right->color == __rbtree_black)) {
                        sibling->color = __rbtree_red;
                        child = parent;
                        parent = parent->parent;
                } else {
                    // case 3: color of sibling's right child is black
                    if(!sibling->right || sibling->right->color == __rbtree_black) {
                        sibling->color = __rbtree_red;
                        if(sibling->left != nullptr) {
                            sibling->left->color = __rbtree_black;
                        }
                        right_rotate(sibling);
                        sibling = parent->right;
                    }
                    // case 4: color of sibling's left is black
                    sibling->color = parent->color;
                    parent->color = __rbtree_black;
                    if(sibling->right) {
                        sibling->right->color = __rbtree_black;
                    }
                    left_rotate(parent);
                    break;
                }
            } else { // similar as above, only change left to right, right to left
                sibling = parent->left;
                if(sibling->color == __rbtree_red) {
                    sibling->color = __rbtree_black;
                    parent->color = __rbtree_red;
                    right_rotate(parent);
                    sibling = parent->left;
                }
                if((!sibling->right || sibling->right->color == __rbtree_black) &&
                    (!sibling->left || sibling->left->color == __rbtree_black)) {
                        sibling->color = __rbtree_red;
                        child = parent;
                        parent = parent->parent;
                } else {
                    if(!sibling->left || sibling->left->color == __rbtree_black) {
                        sibling->color = __rbtree_red;
                        if(sibling->right != nullptr) {
                            sibling->right->color = __rbtree_black;
                        }
                        left_rotate(sibling);
                        sibling = parent->left;
                    }
                    sibling->color = parent->color;
                    parent->color = __rbtree_black;
                    if(sibling->left != nullptr) {
                        sibling->left->color = __rbtree_black;
                    }
                    right_rotate(parent);
                    break;
                }
            } 
        }
        if(child != nullptr) {
            child->color = __rbtree_black;
        }
    }

    template<typename T, typename Compare, typename Alloc>
    void rbtree<T, Compare, Alloc>::dfs_clear(link_type root) {
        // dfs clear left and right
        if(root->left) {
            dfs_clear(root->left);
        }
        if(root->right) {
            dfs_clear(root->right);
        }
        Alloc().destroy(root);
        Alloc().deallocate(root);
    }

    // interface for debug
    template<typename T, typename Compare, typename Alloc>
    bool rbtree<T, Compare, Alloc>::is_rbtree() {
        if(empty()) return true;
        // if root is not black, return false
        if(root()->color != __rbtree_black) {
            return false;
        }
        // if a node is red and its left or right is not black, return false
        for(auto itr= begin(); itr != end(); ++itr) {
            if(itr.node->color == __rbtree_red) {
                if(itr.node->left != nullptr && itr.node->left->color != __rbtree_black) {
                    return false;
                }
                if(itr.node->right != nullptr && itr.node->right->color != __rbtree_black) {
                    return false;
                }
            }
        }
        link_type curr = root();
        size_type count_of_black = 0;
        // count the number of black nodes in a path
        while(curr != nullptr) {
            if(curr->color == __rbtree_black) {
                ++count_of_black;
            }
            curr = curr->left;
        }
        // NIL node is black
        ++count_of_black;
        // if the number of black nodes in any path are different, return false
        return dfs_is_rbtree(root(), 0, count_of_black);
    }

    template<typename T, typename Compare, typename Alloc>
    bool rbtree<T, Compare, Alloc>::dfs_is_rbtree(link_type root, 
        size_type count_of_black_now, const size_type count_of_black) {
            // if root is black, count plus 1
            if(root->color == __rbtree_black) {
                ++count_of_black_now;
            }
            // left or right is nullptr
            if(root->left == nullptr && root->right == nullptr) {
                ++count_of_black_now;
                if(count_of_black_now != count_of_black) {
                    return false;
                } else {
                    return true;
                }
            } else {
                if(root->left == nullptr) {
                    return count_of_black_now + 1 == count_of_black && dfs_is_rbtree(root->right, count_of_black_now, count_of_black);
                } else if(root->right == nullptr) {
                    return count_of_black_now + 1 == count_of_black && dfs_is_rbtree(root->left, count_of_black_now, count_of_black);
                } else {
                    return dfs_is_rbtree(root->left, count_of_black_now, count_of_black) && dfs_is_rbtree(root->right, count_of_black_now, count_of_black);
                }
            }
    }
}

#endif