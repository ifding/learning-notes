
/*********************************************************************************************/
// class stack:
// using list as base container
// including some basic functions of stack:
//
// 1. bool empty() const;
// 2. size_type size() const;
// 3. reference top() const;
// 4. void push(const_reference x);
// 5. void pop();

#ifndef __STACK_H__
#define __STACK_H__

#include "list.h"

namespace tinySTL
{
    // stack class
    template<typename T>
    class stack{
    protected:
            // use list as base container
            list<T, allocator<__list_node<T>>> base_list;
    public:
            using value_type = T;
            using size_type = unsigned int;
            using reference = T&;
            using const_reference = const T&;

            bool empty() const {
                return base_list.empty();
            }
            size_type size() const {
                return base_list.size();
            }
            reference top() const {
                return base_list.back();
            }
            void push(const_reference x) {
                base_list.push_back(x);
            }
            void pop() {
                base_list.pop_back();
            }
    };
}

#endif