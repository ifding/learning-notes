
## Object-Oriented Idioms


### Identity, Equality, and Aliases

The real meaning of an object's ID is implementation-dependent. In CPython, `id()` returns the memory address of the object. The key point is that the ID is guaranteed to be a unique numeric label, and it will never change during the life of the object.

Identity checks are most often done with the `is` operator, and not by comparing IDs.

> Choosing between == and is

The `==` operator compares the values of objects (the data they hold), while `is` compares their identities.

The `is` operator is faster than `==`, because it cannot be overloaded.

> Copies are shallow by default

```python
>>> l1 = [3, [55, 44], (7, 8, 9)]
>>> l2 = list(l1)
>>> l3 = l1[:]
```

Using the `constructor` or `[:]` produces a `shallow copy` (i.e., the outermost container is duplicated, but the copy is filled with references to the same items held by the original container).


> Function parameters as references

The only mode of parameter passing in Python is `call by sharing`, it means that each formal parameter of the function gets a copy of each reference in the arguments. The parameters inside the function become aliases of the actual arguments.

A function may change any mutable object passed as a parameter, but it cannot change the identity of those objects (i.e., it cannot altogether replace an object with another).


> del and garbage collection

The `del` statement deletes names, not objects. An object may be garbage collected as result of a `del` command, but only if the variable deleted holds the last reference to the object, or if the object becomes unreachable.

In CPython, the primary algorithm for garbage collection is reference counting, each object keeps count of how many references point to it. As soon as that `refcount` reaches zero, the object is immediately destroyed: CPython calls the `__del__` method on the object and then frees the memory allocated to the object.

In CPython 2.0, a generational garbage collection algorithm was added to detect groups of objects involved in reference cycles - which may be unreachable even with outstanding references to them, when all the mutual references are contained within the group.


### A Pythonic object

> Object representations

Every object-oriented language has a least one standard way of getting a string representation from any object. Python has two: `repr()` and `str()`.

> classmethod Versus staticmethod

`classmethod` changes the way the method is called, so it receives the class itself as the first argument, instead of an instance. By convention, the first parameter of a class method should be named `cls` (but Python doesn't care how it's named).

`staticmethod` decorator changes a method so that it reveives no special first argument. In essence, a static method is just like a plain function that happens to live in a class body, instead of being defined at the module level.


### Interfaces: From Protocols to ABCs

> ABCs: abstract base classes


