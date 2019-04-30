
# Classes and Dynamic memory allocation

## Dynamic memory and Classes

### A Review example and static class members

Let's look at a new storage class: the static class member. The vehicle will be a `Stringbad` class.

The class declaration:
```c++
// stringbad.h  -- flawed string class definition
#include <iostream>
#ifndef STRINGBAD_H_
#define STRINGBAD_H_

class StringBad
{
public:
	StringBad();                  // default constructor
	StringBad(const char * s);    // constructor
	~StringBad();				  // destructor
	// friend function
	friend std::ostream & operator<<(std::ostream & os, 
		const StringBad & sb);
private:
	char * str;					// pointer to string
	int len;					// length of string
	static int num_strings;		// number of objects
};
#endif
```

You should note two points about this declaration. First, it uses a pointer-to-char instead a char array to represent name. This means that the class declaration does not allocate storage space for the string itself.

Second, the definition declares the `num_strings` member as belonging to the `static` storage class. A `static` class member has a special property: A program creates only one copy of a static class variable, regradless of the number of objects created. That is, a static member is shared among all objects of that class, much as a phone number might be shared among all members of a family. This is convenient for data that should be private to a class but that should have the same value for all class objects. The `num_strings` member, for example, is intended to keep track of the number of objects created.

The implementation of the class methods:
```c++
// stringbad.cpp  -- StringBad class methods
#include <cstring>
#include "stringbad.h"
using std::cout;

// initializing static class member
int StringBad::num_strings = 0;

// class methods
StringBad::StringBad(const char * s)	// construct StringBad from C string
{
	len = std::strlen(s);		// set size
	str = new char[len+1];		// allot storage
	std::strcpy(str, s);		// initialize pointer
	num_strings++;				// set object count
	cout << num_strings << ": \"" << str 
			<< "\" construct from c string\n";	// for your information
}

StringBad::StringBad()
{
	len = 8;
	str = new char[8];
	std::strcpy(str, "default");	// default string
	num_strings++;
	cout << num_strings <<": \"default\" constructor\n";	//FYI
}

StringBad::~StringBad()					// necessary destructor
{
	cout << "\"" << str << "\" object deleted, ";	//FYI
	--num_strings;						// required
	cout << num_strings << " left\n";	// FYI
	delete [] str;						// required
}

std::ostream & operator<<(std::ostream & os, const StringBad & sb)
{
	os << sb.str;
	return os;
}
```

First, `int StringBad::num_strings = 0;` initaializes the static `num_strings` member to 0. Note that you cannnot initialize a static member variable inside the class declaration. That's because the declaration is a description of how memory is to be allocated but it doesn't allocate memory. 

In the case of a static class member, you initialize the static member independently, with a separate statement outside the class declaration. That's because the static class member is stored separately rather than as part of an object. Note that the initialization statement gives the type and uses the scope operator, but it doesn't use the `static` keyword.

This initialization goes in the methods file, not in the class declaration file. That's because the class declaration is in a header file, and a program may include a header file in several other files. That would result in multiple copies o the initialization statement, which is an error.

> A static data member is declared in the class declaration and is initialized in the file containing the class methods. The scope operator is used in the initialization to indicate to which class the static member belongs. However, if the static member is a `const` integral type or an enumeration type, it can be initailized in the class declaration itself.

Next, notice that each constructor contains the expression `num_strings++`. Also the destructor contains the expression `--num_strings`.

The first constructor with a regular C string. Recall that the class `str` member is just a pointer, so the constructor has to provide the memory for holding a string. 1) using the `strlen()` to compute the length of the sring; 2) using `new` to allocate sufficient space to hold the string, and then it assigns the address of the new memory to the `str` member; 3) using `strcpy()` to copy the passed string into the new memory.

You should realize that the string is not stored in the object, but in heap memory, and the object merely stored information that tells where to find the string.

Note that you do not use this:
```c++
str = s;	// not the way to go
```

This merely stores the address without making a copy of the string. The default constructor behaves similarly, except that it provides a default string.

When a `StringBad` object expires, the str pointer expires. But the memory str pointed to remains allocated unless you use `delete` to free it. Whenever you use `new` in a constructor to allocate memory, you should use `delete` in the corresponding destructor to free that memory. If you use `new []` (with brackets), then you should use `delete []` (with brackets).

Let's look at when and how the `StringBad` constructors and destructors work.
```c++
// compile with stringbad.cpp
#include <iostream>
#include "stringbad.h"
using std::cout;

void callme1(StringBad &);		// pass by reference
void callme2(StringBad);		// pass by value

int main() {
	StringBad str1("Dog");
	StringBad str2("Cat");
	callme1(str1);
    cout << "str1: " << str1 << "\n";	
	callme2(str2);
	cout << "str2: " << str2 << "\n";
	cout << "Initialize one object to another\n";
	StringBad str3 = str1;
	cout << "str3: " << str3 << "\n";
	StringBad str4;	
	str4 = str2;
	cout << "str4: " << str4 << "\n";
	return 0;	
}

void callme1(StringBad & rsb)
{
	cout << "\"" << rsb << "\": String passed by reference\n";
}

void callme2(StringBad sb)
{
	cout << "\"" << sb << "\": String passed by value\n";
}
```

Something messes up these strings before they are deleted. Also the counting is bizarre. Actually, every object is constructed once and destroyed once, so the number of construtor calls should equal the number of destructor calls. For example, consider this line:
```c++
StringBad str3 = str1;
```

What constructor is used here? Not the default constructor, and not the constructor with a `const char *` parpameter. Initialization using this form is another syntax for the following:
```c++
StringBad str3 = StringBad(str1);	// constructor using str1
```

Because `str1` is type `StringBad`, a matching constructor could have this prototype:
```c++
StringBad(const StringBad &);
```

And it turns out that the compiler automatically generates this constructor (called a _copy constructor_ because it makes a copy of an object) if you initialize one object to another.

### Special Member Functions

C++ automatically provides the following member functions:
- A default constructor if you define no constructors
- A default destructor if you don't define one
- A copy constructor if you don't define one
- An assignment operator if you don't define one
- An address operator if you don't define one

The compiler generates definitions for the last three items if a program uses objects in such a way as to require them. For example, if you assign one object to another, the program provides a definition for the assignment operator.

It turns out that the implicit copy constructor and the implicit assignment operator cause the `StringBad` class problems.

#### Default Constructors

If you fail to provide any constructors at all, C++ provides you with a default constructor. For example, suppose you define a `StringBad` class and omit any constructors:
```c++
StringBad::StringBad() { }  	// implicit default constructor
```

It supplies a _defaulted_ default constructor that takes no arguments and that does nothing. It's needed because creating an object always invokes a constructor:
```c++
StringBad sb;		// invokes default constructor
```

The default constructor makes `sb` like an ordinary automatic variable; that is, its value at initialization is unknown.

If you want to define a default constructor explicitly:
```c++
StringBad::StringBad()		// explicit default constructor
{
	len = 0;
	...
}
```

A constructor with arguments still can be a default constructor if all its arguments have default values.
```c++
StringBad(int n = 0) { len = n; }
```

However, you can have only one default constructor. That is, you can't have these two. Otherwise, it will be ambiguous.


#### Copy Constructors

A copy constructor is used to copy an object to a newly created object. That is, it's used during initialization, including passing function arguments by value and not during ordinary assignment. A copy constructor for a class normally has this prototype:
```c++
Class_name(const Class_name &);
```

Note that it takes a constant reference to a class object as its argument. You must know two things about a copy constructor: when it's used and what it does.


#### When a copy constructor is used

A copy constructor is invoked whenever a new object is created and initialized to an existing object of the same kind. The most obvious situation is when you explicitly initialize a new object to an existing object.
```c++
StringBad ditto(motto);			// calls StringBad(const StringBad &)
StringBad metto = motto;		// calls StringBad(const StringBad &)
StringBad also = StringBad(motto);	// calls StringBad(const StringBad &)
StringBad *pstr = new StringBad(motto);  // calls StringBad(const StringBad &)
```

Less obviously, a compiler uses a copy constructor whenever a program generates copies of an object. In particular, it's used when a function passes an object by value or when a function returns an object. Remember, passing by value means creating a copy of the original variable. A compiler also uses a copy constructor whenever it generates temporary objects.

By the way, the fact that passing an object by value invokes invoking a copy constructor is a good reason for passing by reference instead. That saves the time of invoking the constructor and the space for storing the new object.

#### What a default copy constructor does

The default copy constructor performs a member-by-member copy of the nonstatic members (sometimes called _shallow copying_). Each member is copied by value. For examples,
```c++
StringBad metto = motto;
```

amounts to the following (aside from the fact that it doesn't compile because access to private members is not allowed):
```c++
StringBad metto;
metto.str = motto.str;
metto.len = motto.len;
```

The following figure illustrates the action of an implicit copy constructor:

![](../images/stringbad_shallow_copying.png)

### Back to StringBad: Where the copy constructor goes wrong

The default copy constructor doesn't increment the `num_strings` counter. However, the destructor does update the count, and it's invoked upon the demise of all objects, regradless of how they were constructed. The solution is to provide an explicit copy constructor to make a deep copy. 

That is, rather than just copy the address of the string, the copy constructor should duplicate the string and assign the address of the duplicate to the str member. That way, each objects gets its own string rather than referring to another object's string.
```c++
StringBad::StringBad(const StringBad & sb)
{
	num_strings++;		// handle static member update
	len = sb.len;		// same length
	str = new char[len+1];	// allot space
	std::strcpy(str, sb.str);	// copy string to new location
	cout << num_strings << ": \""
			<< "\" copy constructor\n";		// FYI
}
```

The deep copying:

![](../images/stringbad_deep_copying.png)


### Assignment operators

C++ allows class object assignment. It does so by automatically overloading an assignment operator for a class. This operator has the following prototype:
```c++
Class_name & Class_name::operator=(const Class_name &);
```

That is, it takes and returns a reference to an object of the class. For example, here's the prototype for the `StringBad` class:
```c++
StringBad & StringBad::operator=(const StringBad &);
```

#### When an assignment operator is used and what it does

An overload assignment operator is used when you assign one object to another existing object:
```c++
StringBad str4;	
str4 = str2;
```

An assignment operator is not necessarily used when initializeing an object:
```c++
String metoo = motto;	// use copy constructor, possibly assignment, too
```

Two steps: using the copy constructor to create a temporary object and then using assignment to copy the values to the new object. That is, initialization always invokes a copy constructor, and forms using the = operator may also invoke an assignment operator.

Like a copy constructor, an implicit implementation of an assignment operator performs a member-to-member copy. If a member is itself an object of some class, the program uses the assignment operator defined for that class to do the copying for that particular member.

#### Where StringBad assignment goes wrong

Once again, the problem is shallow copying, which causes both `str2.str` and `str4.str` to point to the same address. Thus, when the destructor is called for `str4`, it deletes the string "Cat", and when it's called for `str2`, it attempts to delete the previously deleted string. As mentioned earlier, the effect of attempting to delete previously deleted data is undefined, so it may change the memory contents, and it may cause a program to abort.

#### Fixing assignment

The solution for the problems is to make a deep copy. The implementation is similar to that of copy constructor, but there are some differences:
  - Because the target object may already refer to previously allocated data, the function should use `delete []` to free former obligations.
  - The function should protect against assigning an object to itself; otherwise, the freeing of memory described previously could erase the object's contents before they are reassigned.
  - The function returns a reference to the invoking object.

By returning an object, the function can emulate the way ordinary assignment for built-in types can be chained. That is, if S0, S1, and S2 are `StringBad` objects, you can write the following:
```c++
S0 = S1 = S2;
```

In function notation, this becomes the following:
```c++
S0.operator=(S1.operator=(S2));
```

Here's how you could write an assignment operator for the `StringBad` class:
```c++
StringBad & StringBad::operator=(const StringBad & sb)
{
	if (this == & sb)		// object assigned to itself
		return *this;		// all done
	delete [] str;			// free old string
	len = strlen(sb.str); 
	str = new char[len+1];	// get space for new string
	std::strcpy(str, sb.str);	// copy the string
	return *this;			// return reference to invoking object
}
```

First, the code checks for self-assignment. It does so by seeing if the address of the right-hand side of the assignment (`&s`) is the same as the address of the receiving object (`this`). If so, the program returns `*this` and terminates. 

Otherwise, the function proceeds to free the memory that `str` pointed to. The reason for this is that shortly thereafter `str` will be assigned the address of a new string. If you don't first apply the `delete` operator, the previous string will remain in memory. Because the program no longer has a pointer to the old string, that memory will be wasted.

Next, the program proceeds like a copy constructor, allocating enough space for the new string and then copying the string from the right-hand object to the new location.

When it is finished, the program return *this and terminates. Assignment does not create a new object, so you don't have to adjust the value of the static data member `num_strings`.


## Reference

- C++ Primer Plus (Developer's Library) 6th Edition by Stephen Prata
