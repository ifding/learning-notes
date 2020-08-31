C++ Class Templates


One of the most important lessons: _decompositon_ or _factoring_ - breaking problems down into smaller and smaller pieces and solving each subproblem separately.

At the heart of decompostion is the concept of _generality_ - code should avoid overspecializing on a single problem and should be robust enough to adapt to other situations.

A class _template_ is a class that, like the STL `vector` or `map`, is parameterized over some number of types. In a sense, a class template is a class with a hole in it. When a client uses a template class, she fills in these holes to yield a complete type. You cannot create a variable of type `vector` or `map`, but can create `vector<int>` or `maps<string, string>`.


### Defining a Class Template

If we want to call this struct `MyPair` and have it be parameterized over two types, we can write the following:

```c++
 template <typename FirstType, typename SecondType> struct MyPair {
     FirstType first;
     SecondType second;
 };
```

In many ways, type arguments to a class template are similar to regular arguments to C++ functions. If you work on other C++ code bases, you might see the above class template written as follows:

```c++
 template <class FirstType, class SecondType> struct MyPair {
     FirstType first;
     SecondType second;
 };
```

In this instance, `typename` and `class` are completely equivalent to one another. You can only substitute `class` for `typename` in this instance.

To create an instance of `MyPair` specialized over some particular types:

```c++
MyPair<int, string> one; // A pair of an int and a string.
one.first = 137;
one.second = "Templates are cool!";
```

Classes and structs are closely related to one another, so the syntax for declaring a template class is similar to that for a template `struct`. Suppose, for example, that we wish to implement a class `Stack` akin to the STL `stack` which represents a LIFO container.

```c++
 template <typename T> class Stack {
 public:
     void push(T value);
     T pop();

     size_t size();
     bool empty();

 private:
     deque<T> elems;
 };
```

Notice that we've used the template parameter `T` to parameterize the `deque`. This is perfectly valid, and is quite common when implementing template classes.

The proper way to implement each of the member functions as follows.

```c++
 template <typename T> void Stack<T>::push(T value) {
     elems.push_front(value);
 }
 template <typename T> T Stack<T>::pop() {
     T result = elems.front();
     elems.pop_front();
     return result;
 }

 template <typename T> size_t Stack<T>::size() {
     return elems.size();
 }

 template <typename T> bool Stack<T>::empty() {
     return elems.empty();
 }
```

In fact the STL `stack` implementation is very similar to this one. If you define the function inside the body of the template class, you don't need to repeat the template definition. The reason for this is that inside of the class template, the compiler already knows that `T` is template, and it's not necessary to remind it.

>.h and .cpp files for template classes

One way of doing this is to create a .h file for the template class that contains both the class definition and implementation without creating a matching .cpp file. This is the approach adopted by the C++ standard library.

>Putting the class and its definition inside the header file is a valid way to prevent linker errors, but it seems to violate the principle of interface and implementation.

Suppose that class clients need to iterate over the elements of the `Stack` in the order that they will be removed. If we push elements 1,2,3,4,5 onto the stack, the iteration would visit the elements in the order 5,4,3,2,1. We need to add `begin()` and `end()` functions to the `Stack` class that return iterators over the underlying `deque`.

```c++
 template <typename T> class Stack {
 public:
     void push(T value);
     T pop();

     size_t size();
     bool empty();

     //deque<T>::iterator begin(); // Problem: Illegal syntax.
     typename deque<T>::iterator begin(); // Now correct
     //deque<T>::iterator end();   // Problem: Illegal syntax.
     typename deque<T>::iterator end();   // Now correct

 private:
     deque<T> elems;
 };
```

The problem has to do with the fact that `deque<T>` is a dependent type, a type that "depends" on a template parameters. Due to a somewhat arcane restriction in the C++ language, if you try to access a type nested inside of a dependent type inside of a template class (for example, trying to use the `iterator` type nested inside `deque<T>`), you must preface that type with the `typename` keyword, such as `deque<T>::iterator` or `vector<T>::iterator`.

The implementation of the `begin` and `end` functions are shown here:

```c++
 template <typename T> typename deque<T>::iterator Stack<T>::begin() {
     return elems.begin();
 }

 template <typename T> typename deque<T>::iterator Stack<T>::end() {
     return elems.end();
 }
```

The code `template <typename T>` declares that the membder function implementation is an implementation of a template class's member function. `typename deque<T>::iterator` is the return type of the function, and `Stack<T>::begin()` is the name of the member function and the (empty) parameter list.


### Clarifying Interfaces with const

At its core, C++ is a language based on modifying program state. `int`s get incremented in `for` loops; `vector`s have innumerable calls to `clear`, `resize`, and `push_back`; and console I/O overwrites variables with values read directly from the user.

> const Objects

```c++
 class Point {
 public:
     Point(double x, double y);
     double getX() const;
     double getY() const;
     void setX(double newX);
     void setY(double newY);
     double distanceToOrigin() const;
 private:
     double x, y;
 };
 ```

Note that there is a `const` after the member function declaration, which indicates that the membder function does not modify any of the clas's instance variables. In C++ it;s illegal to mark a constructor `const`, since the typical operation of a constructor runs contrary to the notion of `const`.

Here is one possible implementation of `getX`; similar code can be written for `getY`. `const` is part of function's signature, and C++ treats `getX()` and `getX() const` as two different functions.

```c++
double Point::getX() const {
	return x;
}
``` 

In a `const` member function, all the class's instance variables are treated as `const`. You can read their values, but must not modify them. Similarly, inside a `const` member function, you cannot call other non-const member functions. Because they can modify the class's instance variables.

```c++
void Point::distanceToOrigin() const {
    double dx = getX();   // Legal! getX is const.
    double dy = y;        // Legal! Reading an instance variable.
    dx *= dx;             // Legal! We're modifying dx, which isn't an
                          // instance variable.
    dy *= dy;             // Legal! Same reason as above.
    return sqrt(dx + dy); // Legal! sqrt is a free function that can't
                          // modify the current object.
}
```

Remember, `const` member functions guarantee that the _receiver object_ doesn't change, not that the function doesn't change the values of any variables

> const References

This improves program efficiency by avoiding expensive copy operations. `const` references capture the notion of _looking_ at an object without being able to modify it.

```c++
void PrintVector(const vector<int>& vec); 
```

Whether or not the original `vector` is `const`, inside the `PrintVector` function C++ treats as though it were `const`. It is perfectly safe to treat a non-const object as though it were `const` because the legal operations on a `const` object are a subset of the legal operations on a non-const object.

While it's legal to pass non-const objects to functions accepting const references, you cannot pass `const` objects into functions accpeting non-const references.

Suppose we are given the following prototype for a function called `void DoSomething(int& x);`, each of the following calls to `DoSomething` is illegal:

```c++
DoSomething(137);      // Problem: Cannot pass literal by reference
DoSomething(2.71828);  // Problem: Cannot pass literal by reference

double myDouble;
DoSomething(myDouble); // Problem: int& cannot bind to double
```

In the first case, for example, that `DoSomething` is implemented as follows:

```c++
void DoSomething(int& x) {
    x = 0;
}
```

If we pass 137 directly into `DoSomething`, the line `x = 0` would try to store the value 0 into the integer literal 137, this is clearly nonsensical. Passing 2.71828 to `DoSomething` fails for the same reason. The third call fails because `myDouble` is a `double`, not an `int`.

However, suppose we change the prototype of `DoSomething` to accept its parameter by `const` reference:

```c++
void DoSomething(const int& x);
```

Then all of the following calls to `DoSomething` are perfectly legal:

```c++
DoSomething(137);      // Legal
DoSomething(2.71828);  // Legal

double myDouble;
DoSomething(myDouble); // Legal!
```

Why the difference? In the first case, we might accidentally assign a new value to an integer literal; the second case ran into similar problems. In the third case, we cannot bind an `int&` to a `double` because writing a value to that `int&` would result in incorrect behavior. When working with `const` references, none of these problems are possible because the referenced value can't be changed through the reference.


> const and Pointers

pointer-to-const syntax:  __const Type* myPointer__, with the `const` one the left of the star. You can also write as __Type const* myPointer__.

const pointer syntax: __Type* const myPointer__, with the `const` on the right side of the star. A `const` pointer is a pointer that cannot be assigned to point to a different value. You can modify the _pointee_ but not the _pointer_. 

a pointer-to-const (look but don't touch) vs a const pointer (touch, but only touch one thing), one trick for remembering which is which is to read the variable declaration from right-to-left. For example, reading `const Type * ptr` backwards says that "ptr is a pointer to a Type that's const", while `Type * const ptr` read backwards is "ptr is a const pointer to a Type".


> As with references and references-to-const, it is legal to set a pointer-to-const to point to a non-const object. This simply means that the object cannot be modified through the pointer-to-const.


> const_iterator


```c++
 void SubtleFunction(const vector<string>& myVector) {
     if (myVector.empty())
         myVector.clear(); // Error! Calls non-const function.
 }
```

Technically speaking, this function never changes the value of its parameter. However, the C++ compiler will still reject this code, because you invoked `clear` (a non-const member function) on a `const vector`.

A `const` iterator is like a `const` pointer - it can't change what element it iterates over, but it can change the value of the elements it iterates over. Check `const_iterator`.

```c++
void PrintVector(const vector<string>& myVector) {
    for(vector<string>::const_iterator itr = myVector.begin(); // Correct!
        itr != myVector.end(); ++itr)
        cout << *itr << endl;
}
```

How does the `vector` know that it should hand back a `const_iterator` when marked `const` and a regular iterator otherwise? This is a technique known _const-overloading_ and allows a function to have two different behaviors based on whether or not an object is `const`.

```c++
template <typename T> class vector {
public:
    iterator begin();    // return a regular iterator
    iterator end();
    const_iterator begin() const;  // return a const_iterator
    const_iterator end() const;

    void constFunction() const；
    /* ... etc. ... */
private:
    int* elems;  
};
 ```

Consider the following legal implementation of `constFunction`:

```c++
Vector::constFunction() const {
    elems[0] = 137;  // it modifies the value of the elements pointed at by elems
}
```

Because the member function is declared `const`, `elems` acts as a `const` pointer (the pointer can't change) instead of a pointer-to-const (the pointee can't change). bitwise constness vs semantic constness, Bitwise constness means that `const` objects are prohibited from making any bitwise changes to themselves. But the class isn't semantically `const` because the object was able to modify its data.

As a general rule of thumb, avoid returning non-const pointers from member functions that are marked `const`.


> mutable

Given our two conflicing needs - good interface design and good implementation design - how can we strike a balance?

```c++
class GroceryList {
public:
    GroceryList(const string& filename); // Load from a file.

    /* ... other member functions ... */

    string getItemAt(int index) const; // marked const
private:
	/* These data members now mutable. */
    mutable vector<string> data;
    mutable ifstream sourceStream;
};

GroceryList::GroceryList(const string& filename) {
    sourceStream.open(filename.c_str()); // Open the file.
}

string GroceryList::getItemAt(int index) {
    /* Read in enough data to satisfy the request. If we've already read it
    * in, this loop will not execute and we won't read any data.
    */
    while(index >= data.length()) {
        string line;
        getline(sourceStream, line);

        data.push_back(line);
    }
    return data[index];
}
```

Because `data` and `sourceStream` are both `mutable`, the new implementation of `getItemAt` can now be marked `const`. Once data members have been marked `mutable`, any member function can modify them, so be sure to double-check your code for correctness.


> What is const-correctness?

const-correctness requires that const be applied consistently and pervasively. 

- __Objects are never passed by value.__ Any object that would be passed by value is instead passed by reference-to-const or pointer-to-const.

- __Member functions which do not change state are marked const.__ Similarly, a function that is not marked `const` should mutate state somehow.

- __Variable which are set but never modified are marked const.__ Again, a variable not marked `const` should have its value changed at some point.


### Reference

- [Stanford cs106l course reader](https://web.stanford.edu/class/archive/cs/cs106l/cs106l.1184/handouts/full_course_reader.pdf)