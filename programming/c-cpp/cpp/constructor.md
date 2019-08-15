
## Constructor

A constructor is used to initialize an object after it has been allocated by the **new**. We can define multiple constructors with different arguments.

### Implicit C++ member functions

```c++
class Empty{};
```

Actually it becomes something like this:

```c++
class Empty
{
public:
	Empty(){}				      //default constructor
	Empty(const Empty&){}	      //copy constructor
	~Empty(){}				      //destructor
	Empty& operator=(const Empty&){return *this;} //copy assignment operator
                                                    
};
```

The member functions are called only when it's necessary. For example:

```c++
Empty A;				//default constructor
~Empty();				//destructor
Empty B(A);			    //copy constructor (creating a new object)
B = A;			        //copy assignment operator (assign to an existing object)
Empty C = A;            //copy constructor (creating a new object)
```

### Check what is called

```c++
#include <iostream>
using namespace std;

class A
{
public:
    A(int n = 0) : m(n) {cout << "ctor\n";}
    A(const A& a) : m(a.m){cout << "copy\n";}
    A& operator=(const A& a) {
	if(this != &a) m = a.m;
	cout << "assign =\n";
	return *this;
    }
    ~A(){cout << "dtor\n";}
private:
    int m;
};

void foo(const A &a1, const A &a2 = A()){}

int main()
{
    A a(415);         // ctor
    A b = 7;          // ctor
    A c(a);           // copy
    A &d; = c;          
    A e = b;          // copy
    b = d;            // assign = 
    A *p = new A(c);  // copy
    A *q = &a;         
    foo(911);         // ctor, ctor
    delete p;         // dtor

    return 0;
}
```


In the foo(911), the second parameter is obvious that the call **A()** in **const A &a2; =A()** will invoke constructor, the first parameter will also call constructor because the constructor converts the integer **911** to a temporary object of **A**, and use the value for initialization. This makes sense because a **A** object represents a single integer member **m**, thus we expect the class provides way to convert an integer to a **A** object.

### Default Constructor

The default constructor is defined as the constructor that can be called with no arguments.

```c++
class MyClass
{
    int n;
public:
    MyClass();     // default constructor declared
};
```

If we do not explicitly define one, the compiler will automatically generate a default constructor for us. It is automatically invoked in the following examples:

1. When an object value is declared with no argument list, e.g. `MyClass A`; or allocated dynamically with no argument list, e.g. `new MyClass`; the default constructor is used to initialize the object.

2. When an array of objects is declared, e.g. `MyClass A[5]`; or allocated dynamically, e.g. `new MyClass[5]`; the default constructor is used to initialize all the elements.

![](http://www.bogotobogo.com/cplusplus/images/constructor/default_constructor_heap.png)

```c++
#include <iostream>
class MyClass 
{
public:
     MyClass() { std::cout << "default constructor" << std::endl; }
};

int main(int argc, char** argv)
{
     MyClass* myObjArray = new MyClass[5];
}
```

With the following output:

```
default constructor
default constructor
default constructor
default constructor
default constructor
```

3. When a derived class constructor does not explicitly call the base class constructor in its initializer list, the default constructor for the base class is called.

4. When a class constructor does not explicitly call the constructor of one of its object-valued fileds in its initializer list, the default constructor for the field's class is called.

In the standard library, certain containers "fill in" values using the default constructor when the value is not given explicitly, e.g. `vector<MyClass>(5)`; initializes the vector with 5 elements, which are filled with the default-constructed value of our type.

If some constructors are defined, but they are all non-default, the compiler will NOT implicitly define a default constructor.

```c++
class MyClass 
{
private:
    int n;
public:
    MyClass(int m);                // A Constructor     
};

MyClass :: MyClass(int m)
{
    n = m;
}
int main()
{
    MyClass objectA(5);           // Constructor Called           
    MyClass objectB;              // Error: no appropriate default constructor available            
    return 0;
}
```

Most of the class we write will have one or more constructors, a destructor and a copy assignment operator.

```c++
//stringbuilder.h
#include <iostream>

class StringBuilder
{
private:
	static int counter;
	char *str;
public:
	StringBuilder();
	StringBuilder(const char *);
	~StringBuilder();
	const char *getString();
};

//stringbuilder.cpp
#include <cstring>
#include "stringbuilder.h"

int StringBuilder::counter = 0;

StringBuilder::StringBuilder()
{
	const char *defaultStr = "Default String";
	int len = std::strlen(defaultStr);
	str = new char[len + 1];
	std::strcpy(str,"Default String");
	counter++;
	std::cout <<"Default Constructor counter=" << counter << std::endl;
}

StringBuilder::StringBuilder(const char *s)
{
	int len = std::strlen(s);
	str = new char[len + 1];
	std::strcpy(str,s);
	counter++;
	std::cout <<"Constructor counter=" << counter << std::endl;
}

StringBuilder::~StringBuilder()
{
	counter--;
	std::cout <<"Destructor counter=" << counter << std::endl;
	delete [] str;
}

const char* StringBuilder::getString()
{
	return str;
}

//driver.cpp
#include <iostream>
#include "stringbuilder.h"

int main()
{
	StringBuilder myStringA;  //default constructor
	StringBuilder myStringB("StringB");  //constructor taking const char *
	StringBuilder *myStringC = new StringBuilder("StringC");  
	//StringBuilder myStringD = myStringC;  //copy constructor

	std::cout << "myStringA.str = " << myStringA.getString() << std::endl;
	std::cout << "myStringB.str = " << myStringB.getString() << std::endl;
	std::cout << "myStringC->str = " << myStringC->getString() << std::endl;
	//std::cout << "myStringD.str = " << myStringD.getString() << std::endl;

	delete myStringC;
	return 0;
}
```

The output should look like this:

```c++
Default Constructor counter=1
Constructor counter=2
Constructor counter=3
myStringA.str = Default String
myStringB.str = StringB
myStringC->str = StringC
Destructor counter=2
Destructor counter=1
Destructor counter=0
```

Note that we have a pair of `new[]` and `delete[]` in the two constructors and in the destructor. By placing the `delete[]` in the destructor, we ensure the memory that a constructor allocates with the `new[]` is freed when the object goes away.

### Constructor - limitations

C++ constructors have the following limitations:

1. No return type

A constructor cannot return a result, which means that we cannot signal an error during object initilization. The only way of doing it is to throw an excetption from a constructor.

2. Naming

A constructor should have the same name as the class, which means we cannot have two constructors that both take a single argument.

3. Compile time bound

At the time when we create an object, we must specify the name of a concrete class which is known at compile time. There is no way of dynamic binding constructors at run time.

### Destructor

A class needs a destructor if it acquires resources such as memory that we get from the free store using new, files, locks, thread handles, and sockets.

There can be only one destructor for a class. The compiler will also generate code to automatically call the destructor for all our member variables, in the reverse order they appear in the class declaration.

If a class has a pointer or a reference member, it often needs a destructor and copy operations. A class that needs a destructor almost always a copy constructor and a copy assignment. The reason is simply that if an object has acquired a resource, the default meaning of copy which is shallow copy or memberwise copy is almost certainly wrong.


### Copy Constructor

A copy constructor is a special constructor that creates a new object from an existing object. In other words, a copy constructor is a constructor for a `class/struct` which is used to make a copy of an existing instance.

The following cases invoke copy constructor:

1. When an object is created from another object during initialization (Class A=B) 
2. When an object is created from another object as a parameter to a constructor (Class A(B))
3. When an object is passed by value as an argument to a function (function(Class A))
4. When an object is return from a function (Class A; ... return A;)
5. When an exception is thrown using this object type (Class A; ... throw A;)

The copy constructor should have one of the following forms:

```c++
MyClass(const MyClass&);
MyClass(MyClass& );
MyClass(volatile MyClass&);
MyClass(volatile const MyClass&);
```

The following constructors, however, are not copy constructors even though they could do the same thing as a copy constructor.

```c++
MyClass(MyClass*);
MyClass(const MyClass*);
```

- shallow copy vs deep copy

![](http://www.bogotobogo.com/cplusplus/images/constructor/deep_vs_shallow_copy.png)

```c++
// shallow copy
int *p = new int(99);
int *q = p;    // copy the pointer p
*p = 100;      // change the value of the int pointerd to by p

// deep copy
int *p = new int(99);
int *q = new int(*p);  // allocate a new int before copying the value pointed to by p
*p = 100;              // change the value of the int pointed to by p
```

When our object contains raw pointer, then we need to do a deep copy of the pointer.

### Temporary object - copy constructor or return a reference?

```c++
#include <iostream>
using namespace std;

class Foo
{
	char *buf;
public:
	Foo(char const *b = "default") {
		cout << "Constructor Foo()\n";
		buf = new char[std::strlen(b)+1];
		std::strcpy(buf,b);
	}

	~Foo() { 
		cout << "Destructor ~Foo()\n";
		delete[] buf;
	}
};

Foo Func(Foo f) {
	return f;
}

int main()
{
	Foo f;
	cout << "call Func()\n";
	Func(f);
	return 0;
}
```

output:

```c++
Constructor Foo()
call Func()
Destructor ~Foo()
Destructor ~Foo()
```

When the `Func()` is called, the program may crash or have unexpected result. The problem is that it is trying to delete a pointer we haven't allocated.

If we add the following lines of code in the class, we can see the copy constructor is called.

```c++
Foo(const Foo &, char const *b = "default") {
		cout << "Copy constructor\n";
	}
```

The output:

```c++
Constructor Foo()
call Func()
Copy constructor
Copy constructor
Destructor ~Foo()
....crash
```

How to fix it? Two solutions:

1. Add copy constructor that does memory allocation. By adding following line for copy constructor.

```c++
Foo(const Foo &, char const *b = "default") {
	buf = new char[std::strlen(b)+1];
	cout << "Copy constructor\n";
}
//Constructor Foo()
//call Func()
//Copy constructor
//Copy constructor
//Destructor ~Foo()
//Destructor ~Foo()
//Destructor ~Foo()
```

Then, we get new output with matching constructor/destructor.

2. Return reference to the object instead of object itself.

```c++
Foo& Func(Foo &f;) {
	return f;
}
//Constructor Foo()
//call Func()
//Destructor ~Foo()
```

There are cases when we do not want to create copies of an object. We can prevent it by doing:

- Declare a copy constructor and assignment operator. We don't have to provide implementation, and this prevent the compiler from generating its own default versions.
- Make the copy constructor and assignment operator private.
- Use boost and inherit from `boost::noncopyable`.

### Assignment Operator

The assignment operator is used to assign the value of one object to another object, a=b. It differs from the copy constructor in that **the object being assigned to already exists**.

1. Use a **const** reference for the right-hand operand.
2. Return `*this` as a reference to allow operator chaining.
3. Destroy any existing state before setting the new state.
4. Check for self-assignment (a = a) by comparing `this` to `&rhs;`.

```c++
#include <string>

class Array
{
public:
	Array();  // default constructor
	explicit Array(int size);  // non-default constructor
	~Array();  // destructor
	Array(const Array& arr);  // copy constructor
	Array& operator=(const Array& arr);  // assignment operator
private:
	int mSize;
	std::string *mArray;
};

#include <iostream>
#include <algorithm>

// default constructor
Array::Array():mSize(0), mArray(NULL) {}

// non-default constructor
Array::Array(int size):mSize(size), mArray(new std::string[size]) {}

// destructor
Array::~Array()
{
	delete[] mArray;
}

// copy constructor
Array::Array(const Array& arr):mSize(arr.mSize), mArray(new std::string[arr.mSize])
{
	std::copy(arr.mArray, arr.mArray + mSize, mArray);
}

// assignment operator
Array& Array::operator=(const Array& arr)
{
	if(this != &arr)  // self assignment ?
	{
		delete[] mArray;  // delete current array first
		mSize = arr.mSize;
		mArray = new std::string[arr.mSize];
		std::copy(arr.mArray, arr.mArray + mSize, mArray);
	}
	return *this;
}
```

Here are the cases when the methods will be called:

```c++
Array a;       // default constructor
Array a(10);   // non-default constructor
Array b(a);    // copy constructor
Array c=a;     // copy constructor because c does not exist yet
b=c;           // assignment operator
```

### Constructor Initialization

A constructor is similar to a function. It has a name, a parameter list, and a function body.
But, unlike a function, a constructor may also hava a constructor **initializer list**. 

```c++
string s("my string");   // initialize s to the character string "my string"
vector<int> v(100);      // make v a vector of 100 integers
```

The standard string's constructor uses a character string as an initial value, while the vector's constructor uses an integer as the initial number of elements.

```c++
#include <iostream>
#include <string>
using namespace std;

class Student
{
public:
	enum {Freshman = 1, Sophomore, Junior, Senior};
	Student(){}

	Student(const string &fn;, const string &ln;, int i, int y = Freshman)
		:first_name(fn), last_name(ln), id(i), year(y)  { }

private:
	const string first_name;
	const string last_name;
	int id;
	int year;
};

int main()
{
	Student s1("John","Doe", 12345, Student::Junior);
	return 0;
}
```

The constructor initializer starts with a colon. It is followed by a comma-separated list of data members each of which is followed by an initializer inside parentheses. The constructor initializes the `first_name` to the value of its `fn` parameter, `last_name` to the value of its `ln`, and so on. The constructor initializer is specified only on the constructor definition, not its declaration.

One of the primary reasons constructor initializers are hard to understand is that it is usually legal to omit the initializer list and assign values to the data members inside the constructor body.

```c++
Student(const string &fn;, const string &ln;, int i, int y = Freshman)  {
	first_name = fn;
	last_name = ln;
	id = i;
 	year = y; 
}
```

This constructor is the new code assigns the members of class Student. It does not explicitly initialize them. Whether there is an explicit initializer or not, the `first_name` and `last_name` members are initialized even before the constructor is executed. This constructor implicitly uses the default `string` constructor to initialize the `first_name` and `last_name` members. When the body of the constructor is executed, the `first_name` and `last_name` members already have values. Those values are overwritten by the assignment inside the constructor body.

Execution of constructor is done in two steps:

1. The initialization phase
2. Body execution phase, data members of class type are **always** initialization phase, regardless of whether the member is initialized explicitly in the constructor initializer list. Initialization happens **before** any the statement execution of the constructor body.

Some members **must** be initialized in the constructor initializer. For such members, assigning to them in the constructor body doesn't work. In other words, members of a class type that do not have a default constructor and members that are **const** or **reference** types **must** be initialized in the constructor initializer.

```c++
#include <iostream>
#include <string>
using namespace std;

class Student
{
public:
	Student(int id)
	{
		studentId = id;
		ss = id;
		rStudentId = studentId;
	}

private:
	int studentId;
	const int ss;
	int &rStudentId;
};

int main()
{
	Student s1(12345);
	return 0;
}
```

Then, we'll get the errors like this:

```c++
'Student::ss' : must be initialized in constructor base/member initializer list
'Student::rStudentId' : must be initialized in constructor base/member initializer list
l-value specifies const object
```

By the time the constructor body begins executing, initialization is complete. So, the only time we can initialize **const** or **reference** data members is to use the constructor initializer:

```c++
Student(int id) : studentId(id), ss(id), rStudentId(studentId) {}
```

For most of the cases, the difference between initialization and assignment is a matter of efficiency.


## Reference

- [C++ TUTORIAL - CONSTRUCTOR - 2016](http://www.bogotobogo.com/cplusplus/constructor.php)
