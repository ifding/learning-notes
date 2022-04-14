/*********************************************************************************************/
// class allocator: a default simple allocacator, including four functions of allocator:
// 1. pointer allocate(const size_type n = 1);
// 2. void construct(pointer ptr, const_reference value);
// 3. void destroy(pointer ptr);
// 4. void deallocate(pointer ptr, size_type n = 1);
//
// class shared_ptr:
// 1. T& operator* ();
// 2. T* operator-> ();
// 3. T* get();
// 4. unsigned int use_count();
// 5. bool unique();
// 6. void swap(shared_ptr<T>& ptr);
//
// class unique_ptr:
// 1. T& operator* ();
// 2. T* operator-> ();
// 3. T* get();
// 4. T* release();
// 5. void reset();
// 6. void reset(T* ptr);
// 7. void swap(unique_ptr<T>& ptr);
//
// class weak_ptr:
// 1. void reset();
// 2. unsigned int use_count();
// 3. bool expired();
// 4. shared_ptr<T> lock();
// 5. void swap(weak_ptr<T>& ptr);

#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <new>
#include "utility.h"

namespace tinySTL 
{
    // template class allocator
    template<typename T>
    class allocator
    {
    public:
            // define type
            using value_type = T;
            using pointer = value_type*;
            using const_pointer = const pointer;
            using reference = value_type&;
            using const_reference = const T&;
            using size_type = unsigned int;
            using difference_type = int;

            // allocates storage
            pointer allocate(const size_type n = 1) {
                return static_cast<pointer>(::operator new(n * sizeof(value_type)));
            }
            // construct one object
            void construct(pointer ptr, const_reference value) {
                new(ptr) value_type(value);
            }
            // destroy one obejct
            void destroy(pointer ptr) {
                if(ptr) ptr->~T();
            }
            // deallocates storage
            void deallocate(pointer ptr) {
                ::operator delete(ptr);
            }
    };

    template<typename T> class weak_ptr;

    // template class shared_ptr
    template<typename T>
    class shared_ptr {
    private:
            // original pointer
            T* __ptr;
            // reference count pointer
            unsigned int* __countptr;
    public:
            friend class weak_ptr<T>;
            // constructor
            shared_ptr() : __ptr(nullptr), __countptr(nullptr) { }
            shared_ptr(T* ptr) : __ptr(ptr), __countptr(new unsigned int) {
                *__countptr = 1;
            }
            explicit shared_ptr(const shared_ptr<T>& ptr) : 
                __ptr(ptr.__ptr), __countptr(ptr.__countptr) {
                    ++(*__countptr);
            }
            explicit shared_ptr(const weak_ptr<T>& ptr) :
                __ptr(ptr.__ptr), __countptr(ptr.__countptr) {
                    ++(*__countptr);               
            }
            shared_ptr(shared_ptr<T>&& ptr) noexcept : 
                __ptr(ptr.__ptr), __countptr(ptr.__countptr) {
                    ptr.__ptr = nullptr;
                    ptr.__countptr = nullptr;
            }

            // assignment operator
            shared_ptr<T>& operator= (const shared_ptr<T>& ptr) {
                if(this != &ptr) {
                    if(__ptr) {
                        --(*__countptr);
                        if(*__countptr == 0) {
                            delete __ptr;
                            delete __countptr;
                            __ptr = nullptr;
                            __countptr = nullptr;
                        }
                    }
                    __ptr = ptr.__ptr;
                    __countptr = ptr.__countptr;
                    ++(*__countptr);
                }
                return *this;
            }

            shared_ptr<T>& operator= (shared_ptr<T>&& ptr) noexcept {
                if(this != &ptr) {
                    if(__ptr) {
                        --(*__countptr);
                        if(*__countptr == 0) {
                            delete __ptr;
                            delete __countptr;
                            __ptr = nullptr;
                            __countptr = nullptr;
                        }
                    }
                    __ptr = ptr.__ptr;
                    __countptr = ptr.__countptr;
                    ptr.__ptr = nullptr;
                    ptr.__countptr = nullptr;
                }
                return *this;
            }

            // deconstructor
            ~shared_ptr() {
                // if count == 0, then deallocate space
                if(__ptr) {
                    --(*__countptr);
                    if(*__countptr == 0) {
                        delete __ptr;
                        delete __countptr;
                        __ptr = nullptr;
                        __countptr = nullptr;
                    }
                }
            }

            // some frequently used functions
            explicit operator bool() const noexcept {
                return __ptr != nullptr;
            }
            T& operator*() {
                return *__ptr;
            }
            T* operator->() {
                return __ptr;
            }
            T* get() {
                return __ptr;
            }
            unsigned int use_count() {
                return *__countptr;
            }
            bool unique() {
                return *__countptr == 1;
            }
            void swap(shared_ptr<T>& ptr) noexcept {
                tinySTL::swap(*this, ptr);
            }
    };

    //template class unique_ptr
    template<typename T>
    class unique_ptr {
    private:
            // original pointer
            T* __ptr;
    public:
            // constructor
            unique_ptr() : __ptr(nullptr) { }
            unique_ptr(T* ptr) : __ptr(ptr) { }
            // cannot copy construct
            unique_ptr(const unique_ptr<T>& ptr) = delete;
            unique_ptr(unique_ptr<T>&& ptr) noexcept : __ptr(ptr.__ptr) {
                ptr.__ptr = nullptr;
            }
            // assignment operator
            // cannot copy assignment
            unique_ptr<T>& operator= (const unique_ptr<T>& ptr) = delete;
            unique_ptr<T>& operator= (unique_ptr<T>&& ptr) noexcept {
                if(this != &ptr) {
                    __ptr = ptr.__ptr;
                    ptr.__ptr = nullptr;
                }
                return *this;
            }

            // deconstructor
            ~unique_ptr() {
                // release __ptr
                if(__ptr)
                    delete __ptr;
            }

            // some frequently used functions
            explicit operator bool() const noexcept {
                return __ptr != nullptr;
            }
            T& operator*() {
                return *__ptr;
            }
            T* operator->() {
                return __ptr;
            }
            T* get() {
                return __ptr;
            }
            T* release() {
                T* ptr = __ptr;
                __ptr = nullptr;
                return ptr;
            }
            void reset() {
                delete __ptr;
                __ptr = nullptr;
            }
            void reset(T* ptr) {
                delete __ptr;
                __ptr = ptr;
            }
            void swap(unique_ptr<T>& ptr) noexcept {
                tinySTL::swap(*this, ptr);
            }
    };

    // template class weak_ptr
    template<typename T>
    class weak_ptr {
    private:
            // original pointer
            T* __ptr;
            // reference count
            unsigned int* __countptr;
    public:
            friend class shared_ptr<T>;
            // constructor
            weak_ptr() : __ptr(nullptr), __countptr(new unsigned int) { }
            weak_ptr(const shared_ptr<T>& ptr) : 
                __ptr(ptr.__ptr), __countptr(ptr.__countptr) { }
            weak_ptr(const weak_ptr<T>& ptr) : 
                __ptr(ptr.__ptr), __countptr(ptr.__countptr) { }           
            weak_ptr(weak_ptr<T>& ptr) noexcept :
                __ptr(ptr.__ptr), __countptr(ptr.__countptr) {
                    ptr.__ptr = nullptr;
                    ptr.__countptr = nullptr;
            }
            // assignment operator
            weak_ptr<T>& operator= (const weak_ptr<T>& ptr) {
                if(this != &ptr) {
                    __ptr = ptr.__ptr;
                    __countptr = ptr.__countptr;
                }
                return *this;
            }
            weak_ptr<T>& operator= (const shared_ptr<T>& ptr) {
                __ptr = ptr.__ptr;
                __countptr = ptr.__countptr;
                return *this;
            }
            weak_ptr<T>& operator= (weak_ptr<T>&& ptr) noexcept {
                if(this != &ptr) {
                    __ptr = ptr.__ptr;
                    __countptr = ptr.__countptr;
                    ptr.__ptr = nullptr;
                    ptr.__countptr = nullptr;
                }
                return *this;
            }

            // deconstructor
            ~weak_ptr() = default;

            // some frequently used functions
            void reset() {
                __ptr = nullptr;
                __countptr = nullptr;
            }
            unsigned int use_count() {
                return *__countptr;
            }
            bool expired() {
                if(__countptr == nullptr) return true;
                return *__countptr == 0;
            }
            shared_ptr<T> lock() {
                return shared_ptr<T>(*this);
            }
            void swap(weak_ptr<T>& ptr) noexcept {
                tinySTL::swap(*this, ptr);
            }
    };
    
}

#endif