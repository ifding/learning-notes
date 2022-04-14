/*********************************************************************************************/
// some common useful functors:
// 1. negate
// 2. plus
// 3. minus
// 4. multiplies
// 5. divides
// 6. modulus
// 7. equal_to
// 8. not_equal_to
// 9. greater
// 10. less
// 11. greater_equal
// 12. less_equal
// 13. logical_and
// 14. logical_or
// 15. logical_not
// 16. hash

#ifndef __FUNCTIONAL_H__
#define __FUNCTIONAL_H__

namespace tinySTL
{
    // some arithmetic functors

    // negate functor
    template<typename T>
    class negate {
    public:
            inline T operator()(const T& x) const {
                return -x;
            }
    };
    // plus functor
    template<typename T>
    class plus {
    public:
            inline T operator()(const T& x, const T& y) const {
                return x + y;
            }
    };
    // minus functor
    template<typename T>
    class minus {
    public:
            inline T operator()(const T& x, const T& y) const {
                return x - y;
            }
    };
    // multiplies functor
    template<typename T>
    class multiplies {
    public:
            inline T operator()(const T& x, const T& y) const {
                return x * y;
            }
    };
    // divides functor
    template<typename T>
    class divides {
    public:
            inline T operator()(const T& x, const T& y) const {
                return x / y;
            }
    };
    // modulus functor
    template<typename T>
    class modulus {
    public:
            inline T operator()(const T& x, const T& y) const {
                return x % y;
            }
    };

    // six relational functors
    // equal to functor
    template<typename T>
    class equal_to {
    public:
            inline bool operator()(const T& x, const T& y) const {
                return x == y;
            }
    };
    // not equal to functor
    template<typename T>
    class not_equal_to {
    public:
            inline bool operator()(const T& x, const T& y) const {
                return x != y;
            }
    };
    // greater functor
    template<typename T>
    class greater {
    public:
            inline bool operator()(const T& x, const T& y) const {
                return x > y;
            }
    };
    // less functor
    template<typename T>
    class less {
    public:
            inline bool operator()(const T& x, const T& y) const {
                return x < y;
            }
    };
    // greater equal functor
    template<typename T>
    class greater_equal {
    public:
            inline bool operator()(const T& x, const T& y) const {
                return x >= y;
            }
    };
    // less equal functor
    template<typename T>
    class less_equal {
    public:
            inline bool operator()(const T& x, const T& y) const {
                return x <= y;
            }
    };
    
    // three logical functors
    // logical and functor
    template<typename T>
    class logical_and {
    public:
            inline bool operator()(const T& x, const T& y) const {
                return x && y;
            }
    };
    // logical or functor
    template<typename T>
    class logical_or {
    public:
            inline bool operator()(const T& x, const T& y) const {
                return x || y;
            }
    };
    // logical not functor
    template<typename T>
    class logical_not {
    public:
            inline bool operator()(const T& x) const {
                return !x;
            }
    };

    // hash functors, using FNV-1a hash function
    inline unsigned int _Hash_seq(const unsigned char* str, unsigned int count) {
        const unsigned int FNV_offset_basis = 2166136261U;
        const unsigned int FNV_prime = 16777619U;

        unsigned int result = FNV_offset_basis;
        for(unsigned int next = 0; next < count; ++next) {
            result ^= (unsigned int)str[next];
            result *= FNV_prime;
        }
        return result;
    }

    template<typename Key>
    class hash {
    public:
            using size_type = unsigned int;
            inline size_type operator()(const Key& key) {
                return _Hash_seq((const unsigned char*)&key, sizeof(Key));
            }
    };
        
}

#endif