/*********************************************************************************************/
// class iterator
// using type_traits in advance and distance

#ifndef __ITERATOR_H__
#define __ITERATOR_H__

namespace tinySTL
{
    // five iterator categories
    class input_iterator_tag {};
    class ouput_iterator_tag {};
    class forward_iterator_tag: public input_iterator_tag {};
    class bidirectional_iterator_tag: public forward_iterator_tag {};
    class random_access_iterator_tag: public bidirectional_iterator_tag {};

    // template class iterator
    template<typename Category, typename T, typename Distance = int,
            typename Pointer = T*, typename Reference = T&>
    class iterator
    {
    public:
            using iterator_category = Category;
            using value_type = T;
            using difference_type = Distance;
            using pointer = Pointer;
            using reference = Reference;
    };

    // iterator traits
    template<typename Iterator>
    class iterator_traits {
    public:
            using iterator_category = typename Iterator::iterator_category;
            using value_type = typename Iterator::value_type;
            using difference_type = typename Iterator::difference_type;
            using pointer = typename Iterator::pointer; 
            using reference = typename Iterator::reference;
    };

    // template partial specialization for native const pointer
    template<typename T>
    class iterator_traits<const T*> {
    public:
            using iterator_category = random_access_iterator_tag;
            using value_type = T;
            using difference_type = int;
            using pointer = const T*;
            using reference = const T&;
    };

    // template partial specialization for native pointer
    template<typename T>
    class iterator_traits<T*> {
    public:
            using iterator_category = random_access_iterator_tag;
            using value_type = T;
            using difference_type = int;
            using pointer = T*;
            using reference = T&;            
    };

    // decide category of specific iterator
    template<typename Iterator>
    inline typename iterator_traits<Iterator>::iterator_category
        iterator_category(const Iterator&) {
            using category = typename iterator_traits<Iterator>::iterator_category;
            return category();
    }

    // decide distance type of specific iterator
    template<typename Iterator>
    inline typename iterator_traits<Iterator>::difference_type*
        distance_type(const Iterator&) {
            return static_cast<typename iterator_traits<Iterator>::difference_type*>(nullptr);
    }

    // decide value type of specific iterator
    template<typename Iterator>
    inline typename iterator_traits<Iterator>::value_type*
        value_type(const Iterator&) {
            return static_cast<typename iterator_traits<Iterator>::value_type*>(nullptr);
    }

    // distance function
    template<typename InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
        __distance(InputIterator first, InputIterator last, input_iterator_tag) {
            typename iterator_traits<InputIterator>::difference_type n = 0;
            while (first != last) {
                ++first;
                ++n;
            }
            return n;
    }

    // distance function, for RandomAccessIterator
    template<typename RandomAccessIterator>
    inline typename iterator_traits<RandomAccessIterator>::difference_type
        __distance(RandomAccessIterator first, RandomAccessIterator last, 
            random_access_iterator_tag) {
                return last - first;
    }

    // for any type of Iterator, return distance
    template<typename InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
        distance(InputIterator first, InputIterator last) {
            using category = typename iterator_traits<InputIterator>::iterator_category;
            return __distance(first, last, category());
    }

    // advance function
    template<typename InputIterator, typename Distance>
    inline void __advance(InputIterator& i, Distance n, input_iterator_tag) {
        while (n--) {
            ++i;
        }
    }

    // advance function, for BidirectionalIterator
    template<typename BidirectionalIterator, typename Distance>
    inline void __advance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag) {
        if(n >= 0) {
            while(n--) {
                ++i;
            }
        } else {
            while(n++) {
                --i;
            }
        }
    }

    // advance functions, for RandomAccessIterator
    template<typename RandomAccessIterator, typename Distance>
    inline void __advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag) {
        i += n;
    }

    // for any type of Iterator, advance n steps
    template<typename InputIterator, typename Distance>
    inline void advance(InputIterator& i, Distance n) {
        __advance(i, n, iterator_category(i));
    }
}

#endif