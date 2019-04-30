
## Explicit Constructor in C++

A C++ constructor acts like an implicit type converter whenever it is needed. In the example below, the `Foo(int)` constructor acts like an int-to-Foo type converter for the function `doSomething`.

```c++
class Foo
{
public:
    Foo( int x ) { _val = x; }
private:
    int _val;
};
 
void doSomething( const Foo &foo ) {}
 
doSomething( 10 ); // Acts like doSomething( Foo( 10 ) );
```

Only constructors with one non-default parameter can act like implicit convertors. That is, a constructor with only one parameter, which is not a default parameter. Or a constructor with many parameters, where the first parameter is not default and the rest of the parameters are default parameters. In the example below, the constructor with many parameters also acts like an implicit int-to-Foo convertor since all but one are default parameters.

```c++
class Foo
{
public:
    Foo( int x, int y = 99, int z = 999 ) { _val = x; }
private:
    int _val;
};
 
void doSomething( const Foo &foo ) {}
doSomething( 10 );
```

With the inherent complexity of C++, most programmers do not want such implicit conversions happening under the hood. To prevent this from happening, the **explicit** qualifier can be used on such constructors.

```c++
class Foo
{
public:
    explicit Foo( int x ) { _val = x; }
private:
    int _val;
};
 
void doSomething( const Foo&amp; foo ) {}

doSomething( 10 );        // Error!
doSomething( Foo( 10 ) ); // OK
```

### More examples

The compiler is allowed to make one implicit converstion to resolve the parameters to a function. What this means is that the compiler can use constructors callable with a **single parameter** to convert from one tyep to another in order to get the right type for a parameter.

```c++
class Foo
{
public:
  // single parameter constructor, can be used as an implicit conversion
  Foo (int foo) : m_foo(foo) { }
  int GetFoo () { return m_foo; }

private:
  int m_foo;
};
void DoBar (Foo foo)
{
  int i = foo.GetFoo ();
}
int main ()
{
  DoBar (42);
}
```


The argument is not a `Foo` object, but an `int`. However, there exists a constructor for `Foo` that takes an `int` so this constructor can be used to convert the parameter to the correct type.

The compiler is allowed to do this once for each parameter.

Prefixing the `explicit` keywords to the constructor prevents the compiler from using that constructor for implicit conversions. Adding it to the above class will create a compiler error at the function call `DoBar(42)`. It is now necessary to call for conversion explicitly with `DoBar(Foo(42))`.

```c++
#include <iostream>
 
using namespace std;
 
class Complex
{
private:
    double real;
    double imag;
 
public:
    // Default constructor
    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}
 
    // A method to compare two Complex numbers
    bool operator == (Complex rhs) {
       return (real == rhs.real && imag == rhs.imag)? true : false;
    }
};
 
int main()
{
    // a Complex object
    Complex com1(3.0, 0.0);
 
    if (com1 == 3.0)
       cout << "Same";
    else
       cout << "Not Same";
     return 0;
}
```

Output:

```c++
Same
```

We can make the constructor explicit with the help of `explicit` keyword.

```c++
 explicit Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}
```

Output: Compiler Error

```c++
no match for 'operator==' in 'com1 == 3.0e+0'
```

We can still typecast the double values to Complex, but now we have to explicitly typecast it. For example, the following program works fine.

```c++
#include <iostream>
 
using namespace std;
 
class Complex
{
private:
    double real;
    double imag;
 
public:
    // Default constructor
    explicit Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}
 
    // A method to compare two Complex numbers
    bool operator== (Complex rhs) {
       return (real == rhs.real && imag == rhs.imag)? true : false;
    }
};
 
int main()
{
    // a Complex object
    Complex com1(3.0, 0.0);
 
    if (com1 == (Complex)3.0)
       cout << "Same";
    else
       cout << "Not Same";
     return 0;
}
```

Output: The program compiles fine and produces following output.

```c++
Same
```

### A few notes

- The explicit qualifier can be used only with the constructor declaration, not the definition (if the two are separate).
- The explicit qualifier can be used on any constructor. Even the default constructor or constructors with many non-default parameters. It is allowed for these, but does not seem to serve any purpose.

The reason you might want to do this is to avoid accidental construction that can hide bugs. Contrived exampele:

- You have a `MyString(int size)` class with a constructor that constructs a string of the given size. You have a function `print(const MyString&)`, and you call `print(3)` (when you actually intended to call `print("3")`). You expect it to print "3", but it prints an empty string of length 3 instead.

Since explicit prevents such under-the-hood activities, it is a good habit to apply it by default on all constructors. Turn it off only when you actually need an implicit conversion.

## Reference

- [Explicit Constructor in C++](https://codeyarns.com/2010/12/17/explicit-constructor-in-c/)
- [Use of explicit keyword in C++](http://www.geeksforgeeks.org/g-fact-93/)
- [What does the explicit keyword mean?](https://stackoverflow.com/questions/121162/what-does-the-explicit-keyword-mean)
