/*********************************************************************************************/
// class vector:
// using dynamic array as basic data structure
// including some basic functions of vector:
//
// 1. iterator begin() const;
// 2. iterator end() const;
// 3. size_type size() const;
// 4. size_type capacity() const;
// 5. bool empty() const;
// 6. reference operator[] (const size_type i);
// 7. const_reference operator[] (const size_type i) const;
// 8. reference front() const;
// 9. reference back() const;
// 10. void push_back(const_reference x);
// 11. void pop_back();
// 12. iterator insert(const_iterator pos, const_reference x);
// 13. iterator erase(iterator pos);
// 14. void resize(size_type n);
// 15. void clear();

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "memory.h"
#include "iterator.h"

namespace tinySTL
{
    template<typename T, typename Alloc = allocator<T>>
    // template class vector
    class vector {
    public:
            using value_type = T;
            using iterator = value_type*;
            using const_iterator = const iterator;
            using reference = value_type&;
            using const_reference = const T&;
            using size_type = unsigned int;
            using difference_type = int;

            // constructor
            vector() : start(nullptr), finish(nullptr), end_of_storage(nullptr) { }
            vector(size_type n, const_reference value) { 
                fill_initialize(n, value);
            }
            explicit vector(size_type n) {
                fill_initialize(n, value_type());
            }
            vector(const vector<value_type, Alloc>& v);
            vector(vector<value_type, Alloc>&& v) noexcept;
            vector<value_type, Alloc>& operator= (const vector<value_type, Alloc>& v);
            vector<value_type, Alloc>& operator= (vector<value_type, Alloc>&& v) noexcept;

            // destructor
            ~vector() {
                destroy(start, finish);
                deallocate();
            }

            // some common functions of vector
            iterator begin() const {
                return start;
            }
            iterator end() const {
                return finish;
            }
            size_type size() const {
                return end() - begin();
            }
            size_type capacity() const {
                return end_of_storage - begin();
            }
            bool empty() const {
                return begin() == end();
            }
            // operator []
            reference operator[] (const size_type i) {
                return *(begin() + i);
            }
            const_reference operator[] (const size_type i) const {
                return *(begin() + i);
            }
            reference front() const {
                return *begin();
            }
            reference back() const {
                return *(end() - 1);
            }

            void push_back(const_reference x);
            void pop_back();
            iterator insert(const_iterator pos, const_reference x);
            iterator erase(iterator pos);
            void resize(size_type n);
            void clear();

    protected:
            // begining of space that has been used
            iterator start;
            // end of space that has been used
            iterator finish;
            // end of space that can be used
            iterator end_of_storage;
            // internal function, allocate n elements and fill with value
            iterator allocate_and_fill(size_type n, const_reference value) {
                iterator result = allocate(n);
                for(size_type i = 0; i < n; ++i) {
                    construct(result++, value);
                }
                return result - n;
            }
            // iternal function, fill n elements with value, and then adjust pointers
            void fill_initialize(size_type n, const_reference value) {
                start = allocate_and_fill(n, value);
                finish = start + n;
                end_of_storage = finish;
            }

            void construct(iterator itr, const_reference value) {
                Alloc().construct(itr, value);
            }

            void destroy(iterator itr) {
                Alloc().destroy(itr);
            }

            void destroy(iterator first, iterator last) {
                Alloc alloc = Alloc();
                while(first < last) {
                    alloc.destroy(first++);
                }
            }

            iterator allocate(size_type n) {
                return Alloc().allocate(n);
            }

            void deallocate() {
                if(start) {
                    Alloc().deallocate(start);
                }
            }
    };
    // copy constructor
    template<typename T, typename Alloc>
    vector<T, Alloc>::vector(const vector<T, Alloc>& v) {
        // copy from v to *this
        start = allocate(v.size());
        for(size_type i = 0; i < v.size(); ++i) {
            construct(start + i, v[i]);
        }
        finish = start + v.size();
        end_of_storage = finish;
    }
    // move constructor
    template<typename T, typename Alloc>
    vector<T, Alloc>::vector(vector<T, Alloc>&& v) noexcept :
        start(v.start), finish(v.finish), end_of_storage(v.end_of_storage) {
            v.start = nullptr;
            v.finish = nullptr;
            v.end_of_storage = nullptr;
    }

    // copy assignment
    template<typename T, typename Alloc>
    vector<T, Alloc>& vector<T, Alloc>::operator= (const vector<T, Alloc>& v) {
        if(this != &v) {
            // destory and deallocate this
            destroy(begin(), end());
            deallocate();
            // allocate and construct new vector
            start = allocate(v.size());
            for(size_type i = 0; i < v.size(); ++i) {
                construct(start + i, v[i]);
            }
            finish = start + v.size();
            end_of_storage = finish;
        }
        return *this;
    }
    // move assignment
    template<typename T, typename Alloc>
    vector<T, Alloc>& vector<T, Alloc>::operator= (vector<T, Alloc>&& v) noexcept {
        if(this != &v) {
            start = v.start;
            finish = v.finish;
            end_of_storage = v.end_of_storage;
            v.start = nullptr;
            v.finish = nullptr;
            v.end_of_storage = nullptr;
        }
        return *this;
    }

    // push_back
    template<typename T, typename Alloc>
    void vector<T, Alloc>::push_back(const_reference x) {
        // if there is still extra space
        if(finish == end_of_storage) {
            size_type old_size = size();
            // calculate new size
            size_type new_size = old_size ? 2 * old_size : 1;
            iterator new_start = allocate(new_size);
            iterator curr = new_start;
            for(size_type i = 0; i < old_size; ++i) {
                construct(curr++, (*this)[i]);
            }
            destroy(start, finish);
            deallocate();
            start = new_start;
            finish = curr;
            end_of_storage = start + new_size;
        }
        construct(finish++, x);
    }
    // pop_back
    template<typename T, typename Alloc>
    void vector<T, Alloc>::pop_back() {
        destroy(--finish);
    }
    // insert
    template<typename T, typename Alloc>
    typename vector<T, Alloc>::iterator
        vector<T, Alloc>::insert(const_iterator pos, const_reference x) {
            if(finish != end_of_storage) {
                for(auto itr = end(); itr != pos; --itr) {
                    if(itr != end()) 
                        destroy(itr);
                    *itr = *(itr-1);
                }
                construct(pos, x);
                ++finish;
                return pos;
            } else {
                size_type old_size = size();
                size_type new_size = old_size ? 2 * old_size : 1;
                // allocate new space
                iterator new_start = allocate(new_size);
                iterator curr = new_start;
                for(auto itr = begin(); itr != pos; ++itr) {
                    construct(curr++, *itr);
                }
                // insert new value
                auto ret = curr;
                construct(curr++, x);
                for (auto itr = pos; itr != end(); ++itr) {
                    construct(curr++, *itr);
                }
                destroy(begin(), end());
                deallocate();
                start = new_start;
                finish = curr;
                end_of_storage = start + new_size;
                return ret;
            }
    }
    // erase
    template<typename T, typename Alloc>
    typename vector<T, Alloc>::iterator
        vector<T, Alloc>::erase(iterator pos) {
            for(auto itr = pos; itr != end(); ++itr) {
                destroy(itr);
                if(itr+1 != end())
                    construct(itr, *(itr+1));
            }
            --finish;
            return pos;
    }
    // resize
    template<typename T, typename Alloc>
    void vector<T, Alloc>::resize(size_type n) {
        size_type old_size = size();
        // if old_size is larger than n
        if(old_size >= n) {
            for(size_type i = old_size; i > n; --i) {
                pop_back();
            } 
        } else {
            iterator new_start = allocate(n);
            for(size_type i = 0; i < n; ++i) {
                if(i < old_size) {
                    construct(new_start+i, (*this)[i]);
                } else {
                    construct(new_start+i, T());
                }
            }
            destroy(begin(), end());
            deallocate();
            start = new_start;
            finish = new_start + n;
            end_of_storage = finish;
        }
    }
    // clear
    template<typename T, typename Alloc>
    void vector<T, Alloc>::clear() {
        destroy(begin(), end());
        finish = start;
    }
}

#endif