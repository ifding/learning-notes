
## Virtual Functions

When we talk about **virtual function** or **virtual method**, it alwasys in the context of **inheritance** and **polymorphism**. It is a function or method whose behavior can be overridden within an inheriting class by a function with the same signature. In other words, the purpose of virtual functions is to allow customization of derived class implementations.

A virtual method is a method whose implementation is determined at runtime based on the actual type of the invoking object. 

`We don't want static binding for this function. What we do want is the selection of the function to be called at any given point in the program based on the kind of object for which is called.`

`The virtual call is resolved at runtime because the object cannot know whether it belongs to the class the member function is in, or some class derived from it.`

### Virtual member functions and Dynamic binding

- Example 1:

```c++
#include <iostream>

class A {
public:
	void f() { 
		std::cout << "A::f()" << std::endl;
	}
};

class B: public A {
public:
	void f() { 
		std::cout << "B::f()" << std::endl;
	}
};

class C: public B {
public:
	void f() { 
		std::cout << "C::f()" << std::endl;
	}
};

class D: public C {
public:
	// No f()
};

int main()
{
	A *a = new A(); 
	B *b = new B(); 
	C *c = new C();
	D *d = new D();

	a->f();			// A::f()
	b->f();			// B::f()
	c->f();			// C::f()	
	d->f();			// C::f()

	((B *)c)->f();		// B::f()
	((A *)c)->f();		// A::f()
	((A *)b)->f();		// A::f()

	return 0;
}
```

Because **f()** is declared as **nonvirtual**, the invoked method depends on the type used at **compile time**. So, the invoked method is the method of the pointer type:

```c++
((B *)c)->f(); 
```

`c` is a type of `B`, so it invokes the method of Class B and so on.

```c++
d->f();       // C::f()
```

inherited `C::f()` is used because no `f()` was defined in the class `D`.

- Example 2:

```c++
class Base {
public:
	void f();
	virtual void vf();
};

class Derived : public Base {
public:
	void f();
	void vf();
};

#include <iostream>
using namespace std;

void Base::f() {
	cout << "Base f()" << endl;
}

void Base::vf() {
	cout << "Base vf()" << endl;
}

void Derived::f() {
	cout << "Derived f()" << endl;
}

void Derived::vf() {
	cout << "Derived vf()" << endl;
}

int main()
{
	Base b1;
	Derived d1;
	b1.f();
	b1.vf();
	d1.f();
	d1.vf();

	Derived d2;  	// Derived object
	Base* bp = &d2; // Base pointer to Derived object
	bp->f();	// Base f()
	bp->vf();	// which vf()?

	return 0;
}
```

The output of the run is:

```
Base f()
Base vf()
Derived f()
Derived vf()
Base f()
Derived vf()
```

The pointer (or reference) type is known at compile time while object type might be determined at runtime. Interpreting a function call in the source code as executing a particular block of function code is called **binding** the function name.

Binding that takes during compile time is `static binding`. With the virtual function, the binding task is more difficult. The decision of which function to use can't be made at compile time because the compiler doesn't know which object the user is going to choose to make.

`Dynmaic binding`: the compiler has to generate code that allows the correct virtual method to be selected as the code runs.

If **f()** is not declared as **virtual** in the base class, `bp->f()` goes by the pointer type (`Base *`) and invokes `Base::f()`. The compiler can bind `v()` to `Base::f()` at compile time.

If **vf()** is declared as virtual in the base class, `bp->vf()` goes by the object type, here, **Derived** and invokes `Derived::vf()`.

- Example 3:

```c++
#include <iostream>

class Base 
{
public:
	void f() {std::cout << "Base::f()\n";}
	// virtual 
		void vf(){std::cout << "Base::vf()\n";};
};

class Derived: public Base
{
public:
	void f() {std::cout <<"Derived::f()\n";}
	void vf(){std::cout <<"Derived::vf()\n";};
};

int main()
{
	Base b;
	Base *pb = &b;
	Derived d;
	Derived *pd = &d;
	Base *pbd = &d;
	b.f();
	d.f();
	pb->f();
	pd->f();
	pbd->f();
	pbd->vf();

	return 0;
}
```

Output is:

```c++
Base::f()
Derived::f()
Base::f()
Derived::f()
Base::f()
Base::vf()
```

However, if we set the virtual, 

```c++
virtual void vf(){std::cout << "Base::vf()\n";};
```

the output is:

```c++
Base::f()
Derived::f()
Base::f()
Derived::f()
Base::f()
Derived::vf()
```

- Example 4:

```c++
#include <iostream>
#include <vector>
using namespace std;

class A
{
public:
    A(int n = 0) : m(n) {}

public:
    virtual int getVal() const { 
	cout << "A::getVal() = "; 
        return m; 
    }
    virtual ~A() { }

protected:
    int m;
};

class B : public A
{
public:
    B(int n = 0) : A(n) {}

public:
    int getVal() const { 
	cout << "B::getVal() = "; 
	return m + 1; 
    }
};

int main()
{
    const A a(1);
    const B b(3);
    const A *pA[2] = { &a, &b };
    cout << pA[0]->getVal() << endl;
    cout << pA[1]->getVal() << endl;

    vector<A> vA;
    vA.push_back(a);
    vA.push_back(b);
    vector<A>::const_iterator it = vA.begin();
    cout << it->getVal() << endl;
    cout << (it + 1)->getVal() << endl;
    
    return 0;
}
```

The output should look like this:

```c++
A::getVal() = 1
B::getVal() = 4
A::getVal() = 1
A::getVal() = 3
```

Actually, the 3rd and 4th calls are the same as the following:

```c++
 cout << vA[0].getVal() << endl;
 cout << vA[1].getVal() << endl;
```

## Reference

- [VIRTUAL FUNCTIONS - 2014](http://www.bogotobogo.com/cplusplus/virtualfunctions.php#VirtualConstructor)

