#ifndef __TEST_H__
#define __TEST_H__

#include "utility.h"
#include "memory.h"
#include "vector.h"
#include "list.h"
#include "rbtree.h"
#include "set.h"
#include "map.h"
#include "unordered_map.h"
#include "stack.h"
#include "queue.h"
#include "algorithm.h"
#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define UNIT_TEST_BASE(equality, expect, actual) \
    do {\
        test_count++;\
        if (equality)\
            test_pass++;\
        else \
        { \
            std::cerr << __FILE__ << ":" << __LINE__ << ":  except: " \
                << expect << " actual: " << actual << std::endl;\
            main_ret = 1;\
        }\
    } while (0)

#define UNIT_TEST(expect, actual) UNIT_TEST_BASE((expect) == (actual), (expect), (actual))

template<typename T>
void print_elements(const T& container) {
    for(const auto& ele : container)
        std::cout << ele << " ";
    std::cout << std::endl;
}

template<typename T>
void print_element_with_pair(const T& container) {
    for(const auto& pair : container)
        std::cout << "(" << pair.first << ", " << pair.second << ")" << " ";
    std::cout << std::endl;
}

void test_utility() {
    int i = 31, j = 42;
    tinySTL::swap(i, j);
    UNIT_TEST(31, j);
    UNIT_TEST(42, i);

    /*
    int arr0[5] = {1,2,3,4,5};
    int arr1[5] = {5,4,3,2,1};
    tinySTL::swap(arr0, arr1);
    UNIT_TEST(5, arr0[0]);
    */

    tinySTL::pair<std::int32_t, bool> p0; // (0, false)
    tinySTL::pair<std::int32_t, bool> p1(42, true);
    UNIT_TEST(true, p0 < p1); 
    tinySTL::pair<std::int32_t, bool> p2(tinySTL::move(p0));
    p0 = p1;
    UNIT_TEST(42, p0.first);
    p2 = tinySTL::move(p1);
    UNIT_TEST(42, p2.first);

    auto p = tinySTL::make_pair(1, std::string{ "pair" });
    UNIT_TEST(1, p.first);
    UNIT_TEST(std::string{ "pair" }, p.second);
    const auto cp = p;
    UNIT_TEST(1, cp.first);
    p.first = 2;
    UNIT_TEST(true, cp != p);
}

void test_memory() {
    tinySTL::shared_ptr<int> sp0;
    UNIT_TEST(false, static_cast<bool>(sp0));
    tinySTL::shared_ptr<int> sp1(new int(42));
    UNIT_TEST(true, sp1.unique());
    UNIT_TEST(42, *sp1);
    sp0 = sp1;
    UNIT_TEST(2, sp1.use_count());

    tinySTL::unique_ptr<float> up0(new float(1.0));
    UNIT_TEST(1.0, *up0); 
    tinySTL::unique_ptr<float> up1 = tinySTL::move(up0);
    UNIT_TEST(true, up0.get() == nullptr);
    up0.swap(up1);
    UNIT_TEST(1.0, *up0);
    UNIT_TEST(true, up1.get() == nullptr);

    tinySTL::weak_ptr<int> wp0(new int(3));
    UNIT_TEST(true, wp0.expired());
    tinySTL::weak_ptr<int> wp1{sp1};
    UNIT_TEST(2, wp1.use_count());
    tinySTL::weak_ptr<int> wp2 = tinySTL::move(wp1);
    auto sp2 = wp2.lock();
    UNIT_TEST(42, *sp2);
    UNIT_TEST(3, sp2.use_count());
}

void test_vector() {
    srand((unsigned int)time(NULL));
    tinySTL::vector<int> v;
    for(int i = 0; i < 10; ++i) v.push_back(rand()%10);
    UNIT_TEST(true, v[v.size()-1] == v.back());
    UNIT_TEST(16, v.capacity());
    v.pop_back();
    v.erase(v.begin()+3);
    //print_elements(v);
    for(int i = 0; i < 10; ++i) {
        int randn = rand() % 8;
        auto ret_itr = v.insert(v.begin() + randn, -randn);
    }
    tinySTL::vector<int> w(v);
    UNIT_TEST(true, w[0] == v[0]);
    tinySTL::vector<int> u(tinySTL::move(v));
    UNIT_TEST(true, v.empty());
    tinySTL::vector<int> x;
    x = u;
    tinySTL::swap(x, v);
    UNIT_TEST(true, x.empty());
    tinySTL::vector<int> y;
    y = tinySTL::move(w);
    tinySTL::swap(v[0], v[3]);
    UNIT_TEST(true, y.front() == v[3]);
    y.resize(16);
    UNIT_TEST(16, y.size());
    y.clear();
    UNIT_TEST(true, y.empty()); 
}

void test_list() {
    srand((unsigned int)time(NULL));
    tinySTL::list<int> l;
    for(int i = 0; i < 5; ++i) l.push_back(rand()%10);
    UNIT_TEST(5, l.size());
    for(int i = 0; i < 5; ++i) l.push_front(-rand()%10);
    //print_elements(l);
    for(int i = 0; i < 2; ++i) {
        l.pop_back();
        l.pop_front();
    }
    //print_elements(l);
    for(int i = 0; i < 4; ++i) {
        int randn = rand() % 5;
        auto ret_itr = l.insert(l.begin() + randn, -randn);
    }
    for(int i = 0; i < 4; ++i) {
        int randn = rand() % 5;
        auto ret_itr = l.erase(l.begin() + randn);
    }
    UNIT_TEST(6, l.size());
    tinySTL::list<int>w(l);
    tinySTL::list<int>u(tinySTL::move(w));
    UNIT_TEST(true, w.empty());
    w = u;
    tinySTL::list<int> x;
    x = tinySTL::move(u);
    UNIT_TEST(6, x.size());
    UNIT_TEST(0, u.size());
    x.clear();
    UNIT_TEST(0, x.size());
}

void test_rbtree() {
    srand((unsigned int)time(NULL));
    tinySTL::rbtree<int> rbt;
    for (int i = 0; i < 10; ++i) {
        rbt.insert(rand() % 100);
    }
    for (int i = 0; i < 100; ++i) {
        int number = rand() % 100;
        auto itr = rbt.find(number);
        if (itr != rbt.end()) {
            //std::cout << "to remove : " << number << std::endl;
            rbt.erase(itr);
            //print_elements(rbt);
        }
    }
    UNIT_TEST(true, rbt.is_rbtree());
    rbt.clear();
    UNIT_TEST(true, rbt.empty());
}

void test_set() {
    srand((unsigned int)time(NULL));
    tinySTL::set<int> s;
    for (int i = 0; i < 10; ++i) {
        int number = rand() % 100;
        //std::cout << "to insert " << number << " ; ";
        auto itr = s.find(number);
        //if (itr == s.end()) std::cout << "not exists" << std::endl;
        //else std::cout << "already exist : " << *itr << " (obtained by find iterator)" << std::endl;
        s.insert(number);
    }
    //print_elements(s);
    for (int i = 0; i < 100; ++i) {
        int number = rand() % 100;
        auto itr = s.find(number);
        if (itr != s.end()) {
            //std::cout << "to remove : " << number << std::endl;
            s.erase(itr);
            //print_elements(s);
        }
    }
    s.clear();
    UNIT_TEST(true, s.empty());
}

void test_map() {
    srand((unsigned int)time(NULL));
    tinySTL::map<int, int> mp;
    for (int i = 0; i < 10; ++i) {
        int number1 = rand() % 100;
        int number2 = rand() % 100;
        tinySTL::pair<int, int> p = tinySTL::make_pair(number1, number2);
        auto itr = mp.find(p.first);
        //if (itr == mp.end()) std::cout << "not exists" << std::endl;
        //else std::cout << "already exist : " << p.first << ":" << p.second << " (obtained by find iterator)" << std::endl;
        mp.insert(p);
    }
    for (int i = 0; i < 10; ++i) {
        int number = rand() % 100;
        auto itr = mp.find(number);
        if (itr != mp.end()) {
            //print_element_with_pair(mp);
            //std::cout << "to remove key: " << number << std::endl;
            mp.erase(itr);
            //print_element_with_pair(mp);
        }
    }
    mp.clear();
    UNIT_TEST(true, mp.empty());
}

template<typename Key, typename Value>
void print_unordered_map(tinySTL::unordered_map<Key, Value>& um) {
    for(auto itr = um.begin(); itr != um.end(); ++itr) {
        std::cout << itr.get_key() << ":" << *itr << " ";
    }
    std::cout << std::endl;
}

void test_unordered_map() {
    const int max_iteration = 10;
    srand((unsigned int)time(NULL));
    tinySTL::unordered_map<int, int> um;
    for(int i = 0; i < max_iteration; ++i){
        tinySTL::pair<int, int> p(rand() % max_iteration, rand() % max_iteration);
        um.insert(p);
        //std::cout << "insert key-value pair   " << p.first << ":" << p.second << std::endl;
        //print_unordered_map(um);
    }
    for (int i = 0; i < max_iteration; ++i) {
        int key = rand() % max_iteration;
        auto itr = um.find(key);
        if (itr != um.end()) um.erase(itr);
        //std::cout << "deleting key   " << key << std::endl;
        //print_unordered_map(um);
    }
    tinySTL::unordered_map<int, int> um1(um);
    tinySTL::unordered_map<int, int> um2(tinySTL::move(um1));
    tinySTL::unordered_map<int, int> um3;
    um3 = um;
    tinySTL::unordered_map<int, int> um4;
    um4 = tinySTL::move(um2);
    //print_unordered_map(um3);
    um.clear();
    UNIT_TEST(true, um.empty());
}

void test_stack() {
    srand((unsigned int)time(NULL));
    tinySTL::stack<int> s;
    for(int i = 0; i < 5; ++i) { 
        s.push(rand() % 10); 
    }
    UNIT_TEST(5, s.size());
    for(int i = 0; i < 5; ++i) { 
        s.pop();
        //if (!s.empty()) std::cout << "top: " << s.top() << std::endl; 
    }
    UNIT_TEST(true, s.empty());
}

void test_queue() {
    srand((unsigned int)time(NULL));
    tinySTL::queue<int> q;
    for (int i = 0; i < 5; ++i) {
        q.push(rand());
    }
    UNIT_TEST(5, q.size());
    for (int i = 0; i < 5; ++i) {
        q.pop();
    }
    UNIT_TEST(true, q.empty());

    tinySTL::priority_queue<int> pq;
    for (int i = 0; i < 10; ++i) { pq.push(rand() % 100); }
    for (int i = 0; i < 10; ++i) { pq.pop(); } 
    UNIT_TEST(true, pq.empty());
}

void test_algorithm() {
    const int max_iteration = 10;
    std::default_random_engine e((unsigned)time(0));
    std::uniform_int_distribution<unsigned> u(0, 20);
    tinySTL::vector<unsigned> v1;
    for (int i = 0; i < max_iteration; ++i) {
        v1.push_back(u(e));
    }
    //print_elements(v1);
    tinySTL::sort(v1.begin(), v1.end());
    UNIT_TEST(true, std::is_sorted(v1.begin(), v1.end()));
    //print_elements(v1);

    tinySTL::vector<tinySTL::pair<unsigned, unsigned>> v2;
    for (int i = 0; i < max_iteration; ++i) {
        v2.push_back(tinySTL::make_pair(u(e), u(e)));
    }
    //print_element_with_pair(v2);
    tinySTL::merge_sort(v2.begin(), v2.end(), tinySTL::less<tinySTL::pair<unsigned, unsigned>>());
    UNIT_TEST(true, std::is_sorted(v2.begin(), v2.end())); 
    //print_element_with_pair(v2);
}

#endif