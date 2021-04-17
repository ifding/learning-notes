/*********************************************************************************************/
// some common useful algorithms:
//
// using insertion sort/quick sort/heap sort, unstable sort:
// 1. void sort(Iterator First, Iterator Last, Compare cmp);
// 2. void sort(Iterator First, Iterator Last);
//
// using insertion sort/merge sort, stable sort:
// 3. void stable_sort(Iterator First, Iterator Last, Compare cmp);
// 4. void stable_sort(Iterator First, Iterator Last);

#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

#include "functional.h"
#include "iterator.h"
#include "utility.h"
#include "vector.h"

namespace tinySTL
{
    enum { insertion_threshold = 20};

    // copy [first, last) to [first + 1, last + 1), used in insertion sort
    template<typename Iterator>
    inline void __copy_to_next(Iterator first, Iterator last) {
        for(auto itr = last; itr > first; --itr) {
            *itr = *(itr - 1);
        }
    }

    // insertion sort
    template<typename Iterator, typename Compare>
    inline void __insertion_sort(Iterator first, Iterator last, Compare cmp) {
        auto next = first;
        auto value = *first;
        for(auto itr = first + 1; itr != last; ++itr) {
            next = itr;
            value = *itr;
            if(cmp(value, *first)) {
                __copy_to_next(first, itr);
                *first = value;
            } else {
                // unguarded linear insertion
                while(cmp(value, *(next - 1))) {
                    *next = *(next - 1);
                    --next;
                }
                *next = value;
            }
        }
    }

    // quick sort partition, return medium iterator
    template<typename Iterator, typename Compare>
    inline Iterator __quick_sort_partition(Iterator first, Iterator last, Compare cmp) {
        Iterator medium = first + (last - first) / 2;
        // get the medial value of frist, medium, last - 1
        if((cmp(*medium, *first) and cmp(*(last - 1), *medium)) or 
            (cmp(*medium, *(last - 1)) and cmp(*first, *medium))) {
                swap(*first, *medium);
        } else if((cmp(*first, *(last - 1)) and cmp(*(last - 1), *medium)) or 
            (cmp(*medium, *(last - 1)) and cmp(*(last - 1), *first))) {
                swap(*first, *(last - 1));
        }
        auto pivot = *first;
        // partition according the value of pivot
        while(first < last - 1) {
            while(first < last - 1 and !cmp(*(last - 1), pivot)) {
                --last;
            }
            *first = *(last - 1);
            while(first < last - 1 and !cmp(pivot, *first)) {
                ++first;
            }
            *(last - 1) = *first;
        }
        *first = pivot;
        return first;
    }

    // heap sort
    template<typename Iterator, typename Compare>
    inline void __heap_sort(Iterator first, Iterator last, Compare cmp) {
        Iterator now, left, right;
        // make heap
        for(auto itr = (last - first - 1) / 2 + first; itr >= first; --itr) {
            now = itr;
            left = first + (now - first) * 2 + 1;
            right = first + (now - first) * 2 + 2;
            // if has both left and right children
            while(right < last) {
                // exchange parent and left
                if(!cmp(*left, *right) and cmp(*now, *left)) {
                    swap(*left, *now);
                    now = left;
                    left = first + (now - first) * 2 + 1;
                    right = first + (now - first) * 2 + 2;
                } else if(!cmp(*right, *left) and cmp(*now, *right)) {
                    swap(*right, *now);
                    now = right;
                    left = first + (now - first) * 2 + 1;
                    right = first + (now - first) * 2 + 2;
                } else {
                    break;
                }
            }
            // if only has left child
            if(right == last and cmp(*now, *left)) {
                swap(*left, *now);
            }
        }
        // heap sort
        for(auto itr = last - 1; itr >= first; --itr) {
            swap(*first, *itr);
            now = first;
            left = first + 1;
            right = first + 2;
            // if has both left and right children
            while(right < itr) {
                // exchange parent and left
                if(!cmp(*left, *right) and cmp(*now, *left)) {
                    swap(*left, *now);
                    now = left;
                    left = first + (now - first) * 2 + 1;
                    right = first + (now - first) * 2 + 2;
                } else if(!cmp(*right, *left) and cmp(*now, *right)) {
                    // exchange parent and right
                    swap(*right, *now);
                    now = right;
                    left = first + (now - first) * 2 + 1; 
                    right = first + (now - first) * 2 + 2;
                } else {
                    break;
                }
            }
            // if only has left child
            if(right == itr and cmp(*now, *left)) {
                swap(*left, *now);
            }
        }
    }

    // get maximum recursive depth
    inline unsigned int recursive_depth(unsigned int n) {
        unsigned int depth = 0;
        for(; n > 1; n >>= 1, ++depth);
        return depth;
    }

    // sort loop
    template<typename Iterator, typename Compare>
    void __sort_loop(Iterator first, Iterator last, unsigned int depth, Compare cmp) {
        unsigned int count;
        while((count = last - first) > insertion_threshold and depth > 0) {
            --depth;
            Iterator medium = __quick_sort_partition(first, last, cmp);
            if(medium - first < last - medium) {
                __sort_loop(first, medium, depth, cmp);
                first = medium;
            } else {
                __sort_loop(medium, last, depth, cmp);
                last = medium;
            }
        }
        // if exceed recursive depth limitation
        if(depth <= 0) {
            __heap_sort(first, last, cmp);
        // if < insertion_threshold, change to insertion sort
        } else if(count >= 2) {
            __insertion_sort(first, last, cmp);
        }
    }

    // outer interface, using insertion sort/quick sort/heap sort
    template<typename Iterator, typename Compare>
    inline void sort(Iterator first, Iterator last, Compare cmp) {
        if(first < last) {
            __sort_loop(first, last, recursive_depth(last - first), cmp);
        }
    }

    // outer interface, using less as comparison
    template<typename Iterator>
    inline void sort(Iterator first, Iterator last) {
        sort(first, last, less<typename remove_reference<decltype(*first)>::type>());
    }

    // merge, used in merge sort
    template<typename Iterator, typename Compare>
    inline void __merge(Iterator first, Iterator medium, Iterator last, Compare cmp) {
        vector<typename remove_reference<decltype(*first)>::type> v(medium - first);
        auto itr1 = first;
        auto itr2 = v.begin();
        // copy to vector v
        while(itr1 != medium) {
            *itr2++ = *itr1++;
        }
        itr2 = v.begin();
        // merge
        for(auto itr = first; itr != last; ++itr) {
            if(itr2 != v.end() and (itr1 == last or !cmp(*itr1, *itr2))) {
                *itr = *itr2++;
            } else {
                *itr = *itr1++;
            }
        }
    }

    // merge sort
    template<typename Iterator, typename Compare>
    inline void merge_sort(Iterator first, Iterator last, Compare cmp) {
        unsigned int count = last - first;
        if(count < 2) {
            return;
        } else {
            auto medium = first + (last - first) / 2;
            merge_sort(first, medium, cmp);
            merge_sort(medium, last, cmp);
            __merge(first, medium, last, cmp);
        }
    }
    // using less as comparison
    template<typename Iterator>
    inline void merge_sort(Iterator first, Iterator last) {
        merge_sort(first, last, less<typename remove_reference<decltype(*first)>::type>());
    }

}

#endif