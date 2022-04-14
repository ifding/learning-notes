/*********************************************************************************************/
// some common useful tools:
// 1. remove_reference class
// 2. move function
// 3. swap function
// 4. pair class
// 5. make_pair function
//
// the above classes of funcitons are frequently used in containers, adapters, algorithm, etc

#ifndef __UTILITY_H__
#define __UTILITY_H__

namespace tinySTL
{
    // remove_reference traits
    // if the real type is T or T& or T&&, then type = T
    template<typename T>
    class remove_reference {
        public:
                using type = T;
    };
    template<typename T> 
    class remove_reference<T&> {
        public:
                using type = T;
    };
    template<typename T>
    class remove_reference<T&&> {
        public:
                using type = T;
    };

    // move
    // convert a reference from lvalue of rvalue to rvalue
    template<typename T>
    inline typename remove_reference<T>::type&& move(T&& arg) noexcept {
        return (static_cast<typename remove_reference<T>::type&&>(arg));
    }

    // swap
    // swap two arguments
    template<typename T>
    inline void swap(T& lhs, T& rhs) noexcept {
        auto tmp(tinySTL::move(lhs));
        lhs = tinySTL::move(rhs);
        rhs = tinySTL::move(tmp);
    }

    // pair class
    template<typename T1, typename T2>
    struct pair
    {
        T1 first;
        T2 second;
        
        pair() : first(), second() { }
        pair(const pair<T1, T2>& p) : first(p.first), second(p.second) { }
        pair(const pair<T1, T2>&& p) 
            : first(tinySTL::move(p.first)), second(tinySTL::move(p.second)) { }
        pair(const T1& p1, const T2& p2) : first(p1), second(p2) { }

        // copy assign         
        pair& operator=(const pair& rhs) {
            if(this != &rhs) {
                first = rhs.first;
                second = rhs.second;
            }
            return *this;
        }
        
        // move assign
        pair& operator=(pair&& rhs) {
            if(this != &rhs) {
                first = tinySTL::move(rhs.first);
                second = tinySTL::move(rhs.second);
            }
            return *this;
        }
        ~pair() = default;
        
        
    };

    // overloading the comparison operators
    template<typename T1, typename T2>
    bool operator==(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
        return lhs.first == rhs.first and lhs.second == rhs.second;
    }
    template<typename T1, typename T2>
    bool operator<(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
        return lhs.first < rhs.first or (lhs.first == rhs.first and lhs.second < rhs.second);
    }
    template<typename T1, typename T2>
    bool operator!=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
        return !(lhs == rhs);
    }
    template<typename T1, typename T2>
    bool operator>(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
        return rhs < lhs;
    }
    template<typename T1, typename T2>
    bool operator<=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
        return !(lhs > rhs);
    }
    template<typename T1, typename T2>
    bool operator>=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
        return !(lhs < rhs);
    }    

    // make_pair
    // using constructor of class pair
    template<typename T1, typename T2>
    inline pair<T1, T2> make_pair(const T1& p1, const T2& p2) {
        return pair<T1, T2>(p1, p2);
    }
    
}

#endif